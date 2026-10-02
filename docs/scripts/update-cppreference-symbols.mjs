import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const outputPath = path.resolve(__dirname, '../src/data/cppreference-symbols.json');

export async function updateCppReferenceSymbols(options = { maxAgeDays: 7, timeoutMs: 5000, force: false }) {
	const exists = fs.existsSync(outputPath);
	if (exists && !options.force) {
		const stats = fs.statSync(outputPath);
		const ageDays = (Date.now() - stats.mtimeMs) / (1000 * 60 * 60 * 24);
		if (ageDays < options.maxAgeDays) {
			return;
		}
	}

	console.log('[update-symbols] Fetching standard library symbols from cppreference.com...');

	try {
		const controller = new AbortController();
		const timer = setTimeout(() => controller.abort(), options.timeoutMs);

		const mainResponse = await fetch('https://en.cppreference.com/cpp/symbol_index', {
			signal: controller.signal,
			headers: { 'User-Agent': 'Mozilla/5.0 (Documentation Generator)' },
		});

		if (!mainResponse.ok) {
			throw new Error(`HTTP ${mainResponse.status}`);
		}

		const mainHtml = await mainResponse.text();

		const subMatches = [...mainHtml.matchAll(/<a href="(\/cpp\/symbol_index\/([^"#?]+))"/g)];
		const subCategories = [...new Set(subMatches.map((m) => m[2]))].filter(
			(s) => !['macro', 'zombie_names', 'expos'].includes(s)
		);

		const subResponses = await Promise.all(
			subCategories.map((sub) =>
				fetch(`https://en.cppreference.com/cpp/symbol_index/${sub}`, {
					signal: controller.signal,
					headers: { 'User-Agent': 'Mozilla/5.0 (Documentation Generator)' },
				})
					.then((r) => (r.ok ? r.text() : null))
					.catch(() => null)
			)
		);
		clearTimeout(timer);

		const allPages = [
			{ sub: '', html: mainHtml },
			...subCategories.map((sub, i) => ({ sub, html: subResponses[i] })),
		];

		const symbols = {};
		const symbolRegex = /<a href="(\/cpp\/[^"]+)"[^>]*><tt>(.*?)<\/tt><\/a>/g;

		for (const { sub, html } of allPages) {
			if (!html) continue;
			if (sub) {
				symbols[`std::${sub}`] = `https://en.cppreference.com/w/cpp/symbol_index/${sub}`;
			}
			let match;
			while ((match = symbolRegex.exec(html)) !== null) {
				const href = match[1];
				const rawName = match[2]
					.replace(/&lt;/g, '<')
					.replace(/&gt;/g, '>')
					.replace(/&amp;/g, '&')
					.replace(/<[^>]+>/g, '')
					.replace(/\(.*?\)|<.*?>/g, '')
					.trim();

				if (rawName && rawName !== 'std') {
					symbols[`std::${rawName}`] = `https://en.cppreference.com/w${href}`;
					if (sub && !sub.includes('_literals')) {
						symbols[`std::${sub}::${rawName}`] = `https://en.cppreference.com/w${href}`;
					}
				}
			}
		}

		fs.mkdirSync(path.dirname(outputPath), { recursive: true });
		fs.writeFileSync(outputPath, JSON.stringify(symbols, null, '\t'), 'utf-8');
		console.log(`[update-symbols] Successfully updated ${Object.keys(symbols).length} symbols to ${outputPath}`);
	} catch (err) {
		if (exists) {
			console.warn(`[update-symbols] Notice: Could not refresh symbols (${err.message}). Using existing cache.`);
		} else {
			console.log(`[update-symbols] Notice: cppreference.com is unreachable (${err.message}). Building without cppreference links.`);
		}
	}
}

if (process.argv[1] === fileURLToPath(import.meta.url)) {
	const force = process.argv.includes('--force');
	await updateCppReferenceSymbols({ maxAgeDays: 7, timeoutMs: 5000, force });
}
