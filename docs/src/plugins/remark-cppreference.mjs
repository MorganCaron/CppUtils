import { visit } from 'unist-util-visit';
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const symbolsPath = path.resolve(__dirname, '../data/cppreference-symbols.json');
const symbols = fs.existsSync(symbolsPath) ? JSON.parse(fs.readFileSync(symbolsPath, 'utf8')) : {};

function getCppReferenceUrl(rawSymbol) {
	if (!rawSymbol) return null;
	const clean = rawSymbol.trim();
	if (Object.hasOwn(symbols, clean)) return symbols[clean];

	const match = clean.match(/\b(std::[a-zA-Z0-9_:]+)/);
	if (!match) return null;
	const sym = match[1];
	if (Object.hasOwn(symbols, sym)) return symbols[sym];

	const parts = sym.split('::');
	if (parts.length > 2) {
		const parent = parts.slice(0, -1).join('::');
		if (Object.hasOwn(symbols, parent)) return symbols[parent];
	}
	return null;
}

export function remarkCppReference() {
	if (Object.keys(symbols).length === 0) return () => undefined;

	return (tree) => {
		visit(tree, 'inlineCode', (node, index, parent) => {
			if (!parent || parent.type === 'link') return;
			const url = getCppReferenceUrl(node.value);
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

		visit(tree, 'text', (node, index, parent) => {
			if (!parent || parent.type === 'link' || parent.type === 'heading') return;
			const value = node.value;
			if (!value.includes('std::')) return;

			const regex = /\b(std(?:::[a-zA-Z0-9_]+)+)\b/g;
			let match;
			let lastIndex = 0;
			const newChildren = [];

			while ((match = regex.exec(value)) !== null) {
				const sym = match[1];
				const url = getCppReferenceUrl(sym);
				if (!url) continue;

				if (match.index > lastIndex) {
					newChildren.push({
						type: 'text',
						value: value.slice(lastIndex, match.index),
					});
				}

				newChildren.push({
					type: 'link',
					url,
					children: [
						{
							type: 'inlineCode',
							value: sym,
						},
					],
				});

				lastIndex = regex.lastIndex;
			}

			if (newChildren.length > 0) {
				if (lastIndex < value.length) {
					newChildren.push({
						type: 'text',
						value: value.slice(lastIndex),
					});
				}
				parent.children.splice(index, 1, ...newChildren);
				return index + newChildren.length;
			}
		});
	};
}
