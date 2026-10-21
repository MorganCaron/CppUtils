import { visit } from 'unist-util-visit';
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const symbolsPath = path.resolve(__dirname, '../data/cpputils-symbols.json');

function loadSymbols() {
	if (!fs.existsSync(symbolsPath)) return null;
	try {
		return JSON.parse(fs.readFileSync(symbolsPath, 'utf8'));
	} catch {
		return null;
	}
}

function getCppUtilsUrl(rawSymbol, symbols, lang) {
	if (!rawSymbol || !symbols) return null;
	const clean = rawSymbol.trim();
	if (!clean.startsWith('CppUtils::')) return null;

	if (Object.hasOwn(symbols, clean)) return `/${lang}${symbols[clean]}`;

	const withoutTemplate = clean.replace(/<.*>$/, '');
	if (withoutTemplate !== clean && Object.hasOwn(symbols, withoutTemplate)) {
		return `/${lang}${symbols[withoutTemplate]}`;
	}

	return null;
}

export function remarkCppUtils() {
	return (tree, file) => {
		const symbols = loadSymbols();
		if (!symbols || Object.keys(symbols).length === 0) return;

		const filePath = file?.history?.[0] || file?.path || '';
		const isEn = filePath.includes('/en/') || filePath.endsWith('/en.mdx');
		const lang = isEn ? 'en' : 'fr';

		visit(tree, 'inlineCode', (node, index, parent) => {
			if (!parent || parent.type === 'link') return;
			const url = getCppUtilsUrl(node.value, symbols, lang);
			if (!url) return;

			parent.children[index] = {
				type: 'link',
				url,
				children: [
					{
						type: 'inlineCode',
						value: node.value,
					},
				],
			};
		});
	};
}
