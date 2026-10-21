import { visit } from 'unist-util-visit';

function parseTextWithTagsAndAnsi(text) {
	const nodes = [];
	const tokenRegex = /(\[(?:\/)?(?:blue|red|green|yellow|cyan|magenta|dim|bold)\]|\x1b\[[0-9;]*m)/g;
	let lastIndex = 0;
	let match;
	const activeClasses = [];

	function addTextSegment(segment) {
		if (!segment) return;
		nodes.push({
			type: 'element',
			tagName: 'span',
			properties: activeClasses.length > 0 ? { className: [...new Set(activeClasses)] } : {},
			children: [{ type: 'text', value: segment }],
		});
	}

	while ((match = tokenRegex.exec(text)) !== null) {
		if (match.index > lastIndex) {
			addTextSegment(text.slice(lastIndex, match.index));
		}

		const token = match[1];
		if (token.startsWith('\x1b[')) {
			const ansiCodes = token.slice(2, -1).split(';');
			for (const ansiCode of ansiCodes) {
				if (ansiCode === '0' || ansiCode === '') {
					activeClasses.length = 0;
				} else if (ansiCode === '39') {
					for (let i = activeClasses.length - 1; i >= 0; i--) {
						if (['term-red', 'term-green', 'term-blue', 'term-yellow', 'term-magenta', 'term-cyan'].includes(activeClasses[i])) {
							activeClasses.splice(i, 1);
						}
					}
				} else if (ansiCode === '31') {
					activeClasses.push('term-red');
				} else if (ansiCode === '32') {
					activeClasses.push('term-green');
				} else if (ansiCode === '33') {
					activeClasses.push('term-yellow');
				} else if (ansiCode === '34') {
					activeClasses.push('term-blue');
				} else if (ansiCode === '35') {
					activeClasses.push('term-magenta');
				} else if (ansiCode === '36') {
					activeClasses.push('term-cyan');
				} else if (ansiCode === '90' || ansiCode === '2') {
					activeClasses.push('term-dim');
				} else if (ansiCode === '1') {
					activeClasses.push('term-bold');
				} else if (ansiCode === '22') {
					for (let i = activeClasses.length - 1; i >= 0; i--) {
						if (activeClasses[i] === 'term-bold' || activeClasses[i] === 'term-dim') {
							activeClasses.splice(i, 1);
						}
					}
				}
			}
		} else if (token.startsWith('[/')) {
			const tagColor = token.slice(2, -1);
			const className = 'term-' + tagColor;
			const lastIdx = activeClasses.lastIndexOf(className);
			if (lastIdx !== -1) {
				activeClasses.splice(lastIdx, 1);
			}
		} else if (token.startsWith('[')) {
			const tagColor = token.slice(1, -1);
			activeClasses.push('term-' + tagColor);
		}

		lastIndex = tokenRegex.lastIndex;
	}

	if (lastIndex < text.length) {
		addTextSegment(text.slice(lastIndex));
	}

	return nodes;
}

export function remarkTerminal() {
	return (tree) => {
		visit(tree, 'code', (node) => {
			if (node.lang !== 'terminal' && node.lang !== 'console') {
				return;
			}

			const lines = node.value.split('\n');
			const codeChildren = [];

			for (let lineIndex = 0; lineIndex < lines.length; ++lineIndex) {
				const currentLine = lines[lineIndex];
				const parsedLineNodes = parseTextWithTagsAndAnsi(currentLine);
				codeChildren.push(...parsedLineNodes);
				if (lineIndex + 1 < lines.length) {
					codeChildren.push({ type: 'text', value: '\n' });
				}
			}

			const titleMatch = node.meta?.match(/title=["']([^"']+)["']/);
			const title = titleMatch ? titleMatch[1] : (node.lang === 'terminal' ? 'Terminal' : 'Console');

			delete node.lang;
			delete node.meta;
			delete node.value;
			node.type = 'paragraph';
			node.children = [];
			node.data = {
				hName: 'div',
				hProperties: { className: ['terminal-container', 'not-content'] },
				hChildren: [
					{
						type: 'element',
						tagName: 'div',
						properties: { className: ['terminal-header'] },
						children: [
							{
								type: 'element',
								tagName: 'span',
								properties: { className: ['terminal-dot', 'terminal-dot-red'] },
								children: [],
							},
							{
								type: 'element',
								tagName: 'span',
								properties: { className: ['terminal-dot', 'terminal-dot-yellow'] },
								children: [],
							},
							{
								type: 'element',
								tagName: 'span',
								properties: { className: ['terminal-dot', 'terminal-dot-green'] },
								children: [],
							},
							{
								type: 'element',
								tagName: 'span',
								properties: { className: ['terminal-title'] },
								children: [{ type: 'text', value: title }],
							},
						],
					},
					{
						type: 'element',
						tagName: 'pre',
						properties: { className: ['terminal-content'] },
						children: [
							{
								type: 'element',
								tagName: 'code',
								properties: {},
								children: codeChildren,
							},
						],
					},
				],
			};
		});
	};
}

