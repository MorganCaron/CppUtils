import fs from "node:fs";
import path from "node:path";
import { fileURLToPath } from "node:url";
import sax from "sax";
import { updateCppReferenceSymbols } from "./update-cppreference-symbols.mjs";

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const docsRoot = path.resolve(__dirname, "..");
const xmlDir = path.join(docsRoot, "doxygen_xml");

const cpputilsSymbols = {};

if (!fs.existsSync(xmlDir)) {
	console.error(`[generate-api] Error: ${xmlDir} does not exist. Run doxygen first.`);
	process.exit(1);
}

function parseXml(xmlString) {
	return new Promise((resolve, reject) => {
		const parser = sax.parser(true, { trim: false, normalize: false });
		const root = { tag: "root", attributes: {}, children: [], text: "" };
		const stack = [root];

		parser.onopentag = (node) => {
			const element = { tag: node.name, attributes: node.attributes || {}, children: [], text: "" };
			stack[stack.length - 1].children.push(element);
			stack.push(element);
		};

		parser.ontext = (text) => {
			const current = stack[stack.length - 1];
			current.text += text;
			if (stack.length > 1) current.children.push({ tag: "#text", attributes: {}, children: [], text });
		};

		parser.onclosetag = () => {
			stack.pop();
		};

		parser.onend = () => {
			resolve(root.children.find((c) => c.tag !== "#text") || root);
		};

		parser.onerror = (err) => {
			reject(err);
		};

		parser.write(xmlString).close();
	});
}

function findChild(node, tag) {
	return node?.children?.find((c) => c.tag === tag);
}

function findChildren(node, tag) {
	return node?.children?.filter((c) => c.tag === tag) || [];
}

function getAllText(node) {
	if (!node) return "";
	if (node.tag === "#text") return node.text || "";
	if (node.tag === "sp") return " ";
	let text = "";
	for (const child of node.children || []) text += getAllText(child);
	return text;
}

function decodeEntities(str) {
	if (!str) return "";
	return str
		.replace(/&lt;/g, "<")
		.replace(/&gt;/g, ">")
		.replace(/&amp;/g, "&")
		.replace(/&quot;/g, '"')
		.replace(/&apos;/g, "'");
}

function cleanSignature(str) {
	return decodeEntities(str).replace(/\s+/g, " ").trim();
}

// Collapse excessive whitespace in nested template brackets (e.g. vector< int > -> vector<int>)
function cleanTemplateSpaces(str) {
	if (!str) return "";
	let result = str;
	let previous;
	do {
		previous = result;
		result = result
			.replace(/<\s+/g, "<")
			.replace(/\s+>/g, ">")
			.replace(/>\s+>/g, ">>")
			.replace(/\s*,\s*/g, ", ");
	} while (result !== previous);
	return result;
}

function cleanCppType(type) {
	if (!type) return "";
	return cleanTemplateSpaces(decodeEntities(type))
		.replace(/\s+/g, " ")
		.replace(/(\bconst\s+[\w:<>]+|[\w:<>]+)\s+([&*]+)/g, "$1$2")
		.trim();
}

// Normalize parameter formatting (spacing around references, pointers, and variadic packs)
function cleanCppArgs(args) {
	if (!args) return "()";
	return cleanTemplateSpaces(decodeEntities(args))
		.replace(/\s+/g, " ")
		.replace(/\s*=\s*(delete|default|0)\b/g, " = $1")
		.replace(/(\bconst\s+[\w:<>]+|[\w:<>]+)\s+([&*]+)\s*(?:\.\.\.\s*)?(\w+)/g, (match, type, ref, name) => {
			const hasEllipsis = match.includes("...");
			return `${type}${ref}${hasEllipsis ? "... " : " "}${name}`;
		})
		.replace(/(\bconst\s+[\w:<>]+|[\w:<>]+)\s+([&*]+)\s*(?:\.\.\.\s*)?([,)])/g, (match, type, ref, end) => {
			const hasEllipsis = match.includes("...");
			return `${type}${ref}${hasEllipsis ? "..." : ""}${end}`;
		})
		.trim();
}

function sanitizeFileName(name) {
	return name
		.replace(/\(\)/g, "_call")
		.replace(/[()]/g, "_")
		.replace(/[<>]/g, "_")
		.replace(/[*&]/g, "_")
		.replace(/::/g, "_")
		.replace(/,/g, "_")
		.replace(/\s+/g, "_")
		.replace(/\.+/g, "_")
		.replace(/_+/g, "_")
		.replace(/^_|_$/g, "")
		.trim();
}

function sanitizeHtmlId(name) {
	if (!name) return "";
	return name
		.replace(/^~/, "destructor_")
		.replace(/operator\s*([^\w\s]*)/, (m, op) => "operator_" + sanitizeFileName(op))
		.replace(/[^a-zA-Z0-9_-]/g, "_")
		.replace(/_+/g, "_")
		.replace(/^_|_$/g, "");
}

// Split qualified C++ name by "::" while respecting template and function parameter nesting
function splitQualifiedName(rawName) {
	const parts = [];
	let current = "";
	let depth = 0;
	for (let i = 0; i < rawName.length; i++) {
		const character = rawName[i];
		if (character === "<" || character === "(") {
			depth++;
			current += character;
		} else if (character === ">" || character === ")") {
			depth = Math.max(0, depth - 1);
			current += character;
		} else if (character === ":" && depth === 0 && rawName[i + 1] === ":") {
			parts.push(current);
			current = "";
			i++;
		} else current += character;
	}
	if (current) parts.push(current);
	return parts;
}

const sourceFilesCache = new Map();
function getSourceLines(filePath) {
	if (!filePath) return [];
	if (!sourceFilesCache.has(filePath)) {
		if (fs.existsSync(filePath)) sourceFilesCache.set(filePath, fs.readFileSync(filePath, "utf8").split("\n"));
		else sourceFilesCache.set(filePath, []);
	}
	return sourceFilesCache.get(filePath);
}

// Read export module declaration from C++20 module interface file
function extractModuleFromSource(filePath) {
	if (!filePath) return null;
	const lines = getSourceLines(filePath);
	for (let i = 0; i < Math.min(lines.length, 250); i++) {
		const match = /export\s+module\s+([^;]+);/.exec(lines[i]);
		if (match) return match[1].trim();
	}
	return null;
}

function isDetailNamespace(name) {
	return /::(detail|internal|implementation)(::|$)/i.test(name);
}

function formatConceptDeclaration(compounddef, symbolName) {
	const initNode = findChild(compounddef, "initializer");
	let initText = cleanSignature(getAllText(initNode)).trim();
	if (initText) {
		initText = initText.replace(new RegExp(`concept\\s+CppUtils::(?:[A-Za-z0-9_]+::)*${symbolName}\\b`), `concept ${symbolName}`);
		if (!initText.endsWith(";")) initText += ";";
		initText = initText.replace(/^(template\s*<[^>]+>)\s*(concept\b)/, "$1\n$2");
		return cleanTemplateSpaces(initText);
	}
	const templateHeader = extractTemplateParams(compounddef);
	const header = templateHeader ? `${templateHeader}\n` : "";
	return cleanTemplateSpaces(`${header}concept ${symbolName} = ...;`);
}

// Scan tokens to find where a function signature terminates (either '{' body, ';' or constructor ':' initializer)
function findDeclarationEnd(buffer) {
	let inString = false;
	let inChar = false;
	let depthParen = 0;
	let depthBrace = 0;
	let seenParen = false;

	for (let i = 0; i < buffer.length; i++) {
		const character = buffer[i];

		// Escape handling for strings and chars
		if (inString || inChar) {
			if (character === "\\") {
				i++;
				continue;
			}
			if (inString && character === '"') inString = false;
			else if (inChar && character === "'") inChar = false;
			continue;
		}

		if (character === '"') {
			inString = true;
			continue;
		}
		if (character === "'") {
			// Check for C++ digit separator (1'000 or 0xFF'FF)
			if (i > 0 && /[\da-fA-F]/.test(buffer[i - 1]) && i + 1 < buffer.length && /[\da-fA-F]/.test(buffer[i + 1])) continue;
			inChar = true;
			continue;
		}

		// Skip single-line comments // ...
		if (character === "/" && buffer[i + 1] === "/") {
			const newlineIndex = buffer.indexOf("\n", i);
			if (newlineIndex === -1) break;
			i = newlineIndex;
			continue;
		}

		if (character === "(") {
			depthParen++;
			seenParen = true;
		} else if (character === ")") {
			if (depthParen > 0) depthParen--;
		} else if (character === "{") {
			if (depthParen === 0 && depthBrace === 0) {
				// Check if this { belongs to a requires expression (requires requires { ... })
				const prefix = buffer.slice(0, i).trimEnd();
				if (/\brequires(\s*\([^)]*\))?$/.test(prefix)) depthBrace++;
				else return i; // Function body begins here!
			} else depthBrace++;
		} else if (character === "}") {
			if (depthBrace > 0) depthBrace--;
		} else if (character === ";" && depthParen === 0 && depthBrace === 0) {
			// End of declaration without body (or = default / = delete)
			return i;
		} else if (
			character === ":" &&
			depthParen === 0 &&
			depthBrace === 0 &&
			seenParen &&
			buffer[i + 1] !== ":" &&
			(i === 0 || buffer[i - 1] !== ":")
		) {
			// Constructor member initializer list starts here (: member(val))
			return i;
		}
	}

	return -1;
}

// Re-read declaration from source code to preserve modern C++ syntax (trailing return types, requires clauses)
function extractDeclarationFromSource(filePath, startLine, className, memberName) {
	if (!filePath || !fs.existsSync(filePath)) return null;
	const lines = getSourceLines(filePath);
	if (!lines || startLine < 1 || startLine > lines.length) return null;

	let startLineIndex = startLine - 1;
	let buffer = "";
	let foundName = false;

	for (let i = startLineIndex; i < Math.min(lines.length, startLineIndex + 12); i++) {
		const rawLine = lines[i];
		buffer += (buffer ? " " : "") + rawLine.trim();

		if (memberName.startsWith("operator")) {
			if (buffer.includes("operator")) foundName = true;
		} else if (memberName.startsWith("~")) {
			if (buffer.includes("~")) foundName = true;
		} else {
			const escapedMemberName = memberName.replace(/[-[\]{}()*+?.,\\^$|#\s]/g, "\\$&");
			if (new RegExp(`\\b${escapedMemberName}\\b`).test(buffer)) foundName = true;
		}

		if (foundName) {
			const endPos = findDeclarationEnd(buffer);
			if (endPos !== -1) {
				let declarationSignature = buffer.slice(0, endPos).trim();
				// Strip inline keyword
				declarationSignature = declarationSignature.replace(/\binline\s+/g, "");

				// Prepend className
				if (className) {
					if (!declarationSignature.includes(`${className}::${memberName}`)) {
						if (memberName.startsWith("operator")) {
							const opPart = memberName.slice("operator".length).trim();
							const escapedOperator = opPart.replace(/[-[\]{}()*+?.,\\^$|#\s]/g, "\\$&");
							declarationSignature = declarationSignature.replace(new RegExp(`(?<!::)\\boperator\\s*${escapedOperator}`), `${className}::${memberName}`);
						} else if (memberName.startsWith("~")) {
							const escapedClassName = className.replace(/[-[\]{}()*+?.,\\^$|#\s]/g, "\\$&");
							declarationSignature = declarationSignature.replace(new RegExp(`(?<!::)~\\s*${escapedClassName}\\b`), `${className}::~${className}`);
						} else {
							const escapedMemberName = memberName.replace(/[-[\]{}()*+?.,\\^$|#\s]/g, "\\$&");
							declarationSignature = declarationSignature.replace(new RegExp(`(?<!::)\\b${escapedMemberName}\\b`), `${className}::${memberName}`);
						}
					}
				}

				// Format = delete / = default
				declarationSignature = declarationSignature.replace(/\s*=\s*(delete|default|0)\s*;?$/, " = $1;");
				if (!declarationSignature.endsWith(";")) declarationSignature += ";";
				return declarationSignature;
			}
		}
	}
	return null;
}

// Format function signature, preferring source declaration over Doxygen attributes
function formatFunctionSignature(functionNode, className = "") {
	const functionName = cleanSignature(getAllText(findChild(functionNode, "name")));
	const locationNode = findChild(functionNode, "location");
	const filePath = locationNode?.attributes?.file;
	const lineNumber = locationNode?.attributes?.line ? parseInt(locationNode.attributes.line, 10) : 0;

	if (className && filePath && lineNumber > 0) {
		const declarationFromSource = extractDeclarationFromSource(filePath, lineNumber, className, functionName);
		if (declarationFromSource) return declarationFromSource;
	}

	// Fallback to Doxygen attributes
	const returnType = cleanCppType(getAllText(findChild(functionNode, "type")));
	const argumentsString = cleanCppArgs(getAllText(findChild(functionNode, "argsstring")));
	const qualifiedName = className ? `${className}::${functionName}` : functionName;
	return `${returnType ? returnType + " " : ""}${qualifiedName}${argumentsString || "()"};`;
}

// Extract and format template parameters into a C++ template <...> clause
function extractTemplateParams(node) {
	const templateNode = findChild(node, "templateparamlist");
	if (!templateNode) return "";
	const params = findChildren(templateNode, "param").map((p) => {
		const type = cleanSignature(getAllText(findChild(p, "type")));
		const declarationName = cleanSignature(getAllText(findChild(p, "declname")));
		const defaultValue = cleanSignature(getAllText(findChild(p, "defval")));
		let str = [type, declarationName].filter(Boolean).join(" ");
		if (defaultValue) str += ` = ${defaultValue}`;
		return str;
	});
	return params.length > 0 ? `template <${params.join(", ")}>` : "";
}

function autolinkDocText(str, symbolCatalog) {
	if (!str || !symbolCatalog) return str || "";
	return str.replace(
		/\b([A-Za-z_][A-Za-z0-9_]*(?:::[A-Za-z_][A-Za-z0-9_]*)*)\s*(\([^)]*\))/g,
		(fullMatch, symbol, args) => {
			const target = symbolCatalog.get(symbol);
			if (target) {
				return `[\`${symbol}${args}\`](${target.url})`;
			}
			return fullMatch;
		}
	);
}

function renderDocNode(node, lang = "en", refMap = null, symbolCatalog = null) {
	if (!node) return "";
	if (node.tag === "#text") return autolinkDocText(node.text || "", symbolCatalog);
	let result = "";

	for (const child of node.children || []) {
		if (child.tag === "#text") {
			result += autolinkDocText(child.text || "", symbolCatalog);
		} else if (child.tag === "para") {
			const paraText = renderDocNode(child, lang, refMap, symbolCatalog).trim();
			if (paraText) result += `${paraText}\n\n`;
		} else if (child.tag === "parameterlist") {
			// Render function or template parameter documentation as markdown tables
			const isTemplate = child.attributes.kind === "templateparam";
			const colName = isTemplate
				? (lang === "fr" ? "Paramètre template" : "Template parameter")
				: (lang === "fr" ? "Paramètre" : "Parameter");

			const items = findChildren(child, "parameteritem");
			if (items.length > 0) {
				result += `| ${colName} | Description |\n`;
				result += `| :--- | :--- |\n`;
				for (const item of items) {
					const nameNode = findChild(findChild(item, "parameternamelist"), "parametername");
					const descriptionNode = findChild(findChild(item, "parameterdescription"), "para");
					const name = getAllText(nameNode).trim();
					const description = renderDocNode(descriptionNode, lang, refMap, symbolCatalog).trim().replace(/\n+/g, " ");
					if (name) result += `| \`${name}\` | ${description} |\n`;
				}
				result += "\n";
			}
		} else if (child.tag === "simplesect") {
			const kind = child.attributes.kind;
			if (kind === "see") {
				// Transform @see references into guide callouts or external markdown links
				const paras = findChildren(child, "para");
				const targetParas = paras.length > 0 ? paras : [child];
				for (const para of targetParas) {
					const allParaText = getAllText(para).trim();
					const ulink = findChild(para, "ulink") || findChildren(para, "para").map((p) => findChild(p, "ulink")).find(Boolean);
					const rawUrl = (ulink?.attributes?.url || (allParaText.match(/https?:\/\/[^\s)]+/)?.[0] || ""))
						.replace(/&ndash;/g, "--")
						.replace(/&mdash;/g, "---")
						.replace(/\u2013/g, "--")
						.replace(/\u2014/g, "---");

					const docMatch = rawUrl.match(/(?:https?:\/\/[^/]+)?\/?(?:CppUtils)?\/(fr|en)\/([^)\s]+)/);
					if (docMatch) {
						const linkLang = docMatch[1];
						const docPath = docMatch[2];
						if (linkLang !== lang) continue;

						let cleanPath = docPath;
						let cleanHash = "";
						const hashIndex = cleanPath.indexOf("#");
						if (hashIndex !== -1) {
							cleanHash = cleanPath.slice(hashIndex).replace(/\/+$/, "");
							cleanPath = cleanPath.slice(0, hashIndex);
						}
						cleanPath = cleanPath.replace(/\/+$/, "");
						const targetUrl = cleanHash ? `/${linkLang}/${cleanPath}/${cleanHash}` : `/${linkLang}/${cleanPath}/`;

						const labelMatch = allParaText.match(/\(([^)]+)\)/);
						const label = labelMatch ? labelMatch[1].trim() : (lang === "fr" ? "Guide associé" : "Related guide");

						const noteTitle = lang === "fr" ? "Guide associé" : "Related guide";
						const noteText = lang === "fr"
							? `Consultez la section [${label}](${targetUrl}) pour une explication détaillée et des exemples d'utilisation.`
							: `See the [${label}](${targetUrl}) section for detailed explanations and usage examples.`;

						result += `\n:::note[${noteTitle}]\n${noteText}\n:::\n\n`;
						continue;
					}

					const firstParen = allParaText.indexOf("(");
					const lastParen = allParaText.lastIndexOf(")");
					const label = (firstParen !== -1 && lastParen > firstParen)
						? allParaText.slice(firstParen + 1, lastParen).trim()
						: "";
					if (ulink && label) {
						const url = (ulink.attributes?.url || "")
							.replace(/&ndash;/g, "--")
							.replace(/&mdash;/g, "---")
							.replace(/\u2013/g, "--")
							.replace(/\u2014/g, "---");
						const safeLabel = label.replace(/<([^>]+)>/g, "`<$1>`");
						result += `${lang === "fr" ? "\n**Voir aussi :** " : "\n**See also:** "}[${safeLabel}](${url})\n\n`;
						continue;
					}

					const content = renderDocNode(para, lang, refMap, symbolCatalog).trim();
					if (content) {
						result += `${lang === "fr" ? "\n**Voir aussi :** " : "\n**See also:** "}${content}\n\n`;
					}
				}
				continue;
			}
			// Map generic Doxygen command sections (@return, @note, @warning, etc.) to Starlight markdown callouts
			const content = renderDocNode(child, lang, refMap, symbolCatalog).trim();
			if (kind === "return") result += `${lang === "fr" ? "\n**Retourne :** " : "\n**Returns:** "}${content}\n\n`;
			else if (kind === "note") result += `\n:::note\n${content}\n:::\n\n`;
			else if (kind === "tip" || kind === "remark" || kind === "remarks") result += `\n:::tip\n${content}\n:::\n\n`;
			else if (kind === "warning" || kind === "attention") result += `\n:::caution\n${content}\n:::\n\n`;
			else if (kind === "danger" || kind === "error") result += `\n:::danger\n${content}\n:::\n\n`;
			else result += `${content}\n\n`;
		} else if (child.tag === "ulink") {
			// Render external hyperlinks
			const url = (child.attributes?.url || "")
				.replace(/&ndash;/g, "--")
				.replace(/&mdash;/g, "---")
				.replace(/\u2013/g, "--")
				.replace(/\u2014/g, "---");
			const text = renderDocNode(child, lang, refMap, symbolCatalog).trim() || url;
			result += `[${text}](${url})`;
		} else if (child.tag === "ref") {
			// Resolve cross-references to types, classes, and member methods
			const text = getAllText(child).trim();
			const refId = child.attributes?.refid;
			let target = refMap?.get(refId);
			if (!target && refId) {
				const parentRefId = refId.replace(/_1[a-zA-Z0-9]+$/, "");
				target = refMap?.get(parentRefId);
				if (target && child.attributes?.kindref === "member") {
					const memberName = sanitizeHtmlId(text.replace(/\(\)$/, ""));
					target = { url: `${target.url}#${memberName}`, name: target.name };
				}
			}

			if (target) result += `[\`${text}\`](${target.url})`;
			else if (child.attributes?.kindref === "compound" && refId?.startsWith("namespace")) result += text;
			else result += `\`${text}\``;
		} else if (child.tag === "computeroutput") {
			// Autolink inline code spans that match known symbols or user-defined literals
			const codeText = getAllText(child).trim();
			const target = symbolCatalog?.get(codeText);
			if (target) {
				result += `[\`${codeText}\`](${target.url})`;
			} else {
				result += `\`${codeText}\``;
			}
		} else if (child.tag === "programlisting") {
			// Render C++ code snippets with syntax highlighting and line numbers
			const codeLines = findChildren(child, "codeline").map((lineNode) => getAllText(lineNode));
			const code = codeLines.length > 0 ? codeLines.join("\n") : getAllText(child);
			result += `\n\`\`\`cpp showLineNumbers\n${code.trim()}\n\`\`\`\n\n`;
		} else {
			result += renderDocNode(child, lang, refMap, symbolCatalog);
		}
	}

	return decodeEntities(result);
}

const anchorLinkIconSvg = `<span aria-hidden="true" class="sl-anchor-icon"><svg width="16" height="16" viewBox="0 0 24 24" fill="currentColor"><path d="m12.11 15.39-3.88 3.88a2.52 2.52 0 0 1-3.5 0 2.47 2.47 0 0 1 0-3.5l3.88-3.88a1 1 0 1 0-1.42-1.42l-3.88 3.89a4.48 4.48 0 0 0 6.33 6.33l3.89-3.88a1 1 0 0 0-1.42-1.42m8.58-12.08a4.49 4.49 0 0 0-6.33 0l-3.89 3.88a1 1 0 1 0 1.42 1.42l3.88-3.88a2.52 2.52 0 0 1 3.5 0 2.47 2.47 0 0 1 0 3.5l-3.88 3.88a1 1 0 0 0 0 1.42 1 1 0 0 0 1.42 0l3.88-3.89a4.49 4.49 0 0 0 0-6.33M8.83 15.17a1 1 0 0 0 .71.29 1 1 0 0 0 .71-.29l4.92-4.92a1 1 0 1 0-1.42-1.42l-4.92 4.92a1 1 0 0 0 0 1.42"></path></svg></span>`;

function renderMemberCard(anchorId, title, codeBlock, briefDesc, detailedDesc, anchorLabel = "") {
	let output = `<div class="member-card">\n\n`;
	if (anchorId) {
		const label = anchorLabel || `Direct link to ${anchorId}`;
		output += `<div class="member-signature" id="${anchorId}">\n`;
		output += `<a class="member-anchor" href="#${anchorId}" aria-label="${label}" title="${label}">${anchorLinkIconSvg}</a>\n\n`;
		output += `\`\`\`cpp\n${codeBlock}\n\`\`\`\n\n</div>\n\n`;
	} else if (title) {
		output += `### \`${title}\`\n\n\`\`\`cpp\n${codeBlock}\n\`\`\`\n\n`;
	}
	if (briefDesc) output += `${briefDesc}\n\n`;
	if (detailedDesc) output += `${detailedDesc}\n\n`;
	output += `</div>\n\n`;
	return output;
}

function renderNamespaceContent(publicEnums, publicTypes, publicFuncs, publicVars, labels, lang, refMap, symbolCatalog) {
	let markdown = "";

	if (publicEnums.length > 0) {
		markdown += `## ${labels.publicEnums}\n\n`;
		for (const enumNode of publicEnums) {
			const enumName = cleanSignature(getAllText(findChild(enumNode, "name")));
			const isScopedEnum = enumNode.attributes.strong === "yes";
			const enumValues = findChildren(enumNode, "enumvalue").map((valueNode) => {
				const valueName = cleanSignature(getAllText(findChild(valueNode, "name")));
				const valueInitializer = cleanSignature(getAllText(findChild(valueNode, "initializer")));
				return `    ${valueName}${valueInitializer ? " " + valueInitializer : ""}`;
			});
			const enumDefinition = enumValues.length > 0
				? `enum ${isScopedEnum ? "class " : ""}${enumName} {\n${enumValues.join(",\n")}\n};`
				: `enum ${isScopedEnum ? "class " : ""}${enumName} {};`;
			const enumBrief = renderDocNode(findChild(enumNode, "briefdescription"), lang, refMap, symbolCatalog).trim();
			const enumDetailed = renderDocNode(findChild(enumNode, "detaileddescription"), lang, refMap, symbolCatalog).trim();
			markdown += renderMemberCard(null, enumName, enumDefinition, enumBrief, enumDetailed);
		}
	}

	if (publicTypes.length > 0) {
		markdown += `## ${labels.publicTypes}\n\n`;
		for (const typeNode of publicTypes) {
			const typeName = cleanSignature(getAllText(findChild(typeNode, "name")));
			const typeDefinition = cleanTemplateSpaces(cleanSignature(getAllText(findChild(typeNode, "definition"))));
			const typeBrief = renderDocNode(findChild(typeNode, "briefdescription"), lang, refMap, symbolCatalog).trim();
			markdown += renderMemberCard(null, typeName, cleanTemplateSpaces(`${typeDefinition || typeName};`), typeBrief, "");
		}
	}

	if (publicVars.length > 0) {
		markdown += `## ${labels.publicConstants}\n\n`;
		const seenVarIds = new Set();
		for (const variableNode of publicVars) {
			const variableName = cleanSignature(getAllText(findChild(variableNode, "name")));
			const variableType = cleanCppType(getAllText(findChild(variableNode, "type")));
			const variableInitializer = cleanSignature(getAllText(findChild(variableNode, "initializer")));
			const variableDefinition = cleanTemplateSpaces(cleanSignature(getAllText(findChild(variableNode, "definition"))));
			const variableBrief = renderDocNode(findChild(variableNode, "briefdescription"), lang, refMap, symbolCatalog).trim();
			const variableDetailed = renderDocNode(findChild(variableNode, "detaileddescription"), lang, refMap, symbolCatalog).trim();

			let cleanId = sanitizeHtmlId(variableName) || "var";
			if (seenVarIds.has(cleanId)) {
				let overloadCounter = 2;
				while (seenVarIds.has(`${cleanId}_${overloadCounter}`)) overloadCounter++;
				cleanId = `${cleanId}_${overloadCounter}`;
			}
			seenVarIds.add(cleanId);

			const anchorLabel = lang === "fr" ? `Lien direct vers ${cleanId}` : `Direct link to ${cleanId}`;
			const codeBlock = cleanTemplateSpaces(`${variableDefinition || `${variableType} ${variableName}${variableInitializer ? " " + variableInitializer : ""}`};`);
			markdown += renderMemberCard(cleanId, null, codeBlock, variableBrief, variableDetailed, anchorLabel);
		}
	}

	if (publicFuncs.length > 0) {
		markdown += `## ${labels.publicFuncs}\n\n`;
		const seenFuncIds = new Set();
		for (const functionNode of publicFuncs) {
			const functionName = cleanSignature(getAllText(findChild(functionNode, "name")));
			const functionTemplate = extractTemplateParams(functionNode);
			const functionBrief = renderDocNode(findChild(functionNode, "briefdescription"), lang, refMap, symbolCatalog).trim();
			const functionDetailed = renderDocNode(findChild(functionNode, "detaileddescription"), lang, refMap, symbolCatalog).trim();
			const functionSignature = formatFunctionSignature(functionNode, "");

			let cleanId = sanitizeHtmlId(functionName) || "func";
			if (seenFuncIds.has(cleanId)) {
				let overloadCounter = 2;
				while (seenFuncIds.has(`${cleanId}_${overloadCounter}`)) overloadCounter++;
				cleanId = `${cleanId}_${overloadCounter}`;
			}
			seenFuncIds.add(cleanId);

			const anchorLabel = lang === "fr" ? `Lien direct vers ${cleanId}` : `Direct link to ${cleanId}`;
			const codeBlock = functionTemplate && !functionSignature.startsWith("template") ? `${functionTemplate}\n${functionSignature}` : functionSignature;
			markdown += renderMemberCard(cleanId, null, codeBlock, functionBrief, functionDetailed, anchorLabel);
		}
	}

	return markdown;
}

// Index types, classes, structs, and free functions for cross-referencing and autolinking
function buildSymbolCatalog(compounds, namespaceCompounds, lang) {
	const refMap = new Map();
	const symbolCatalog = new Map();
	const slugCounts = new Map();
	const primaryTemplates = new Map();

	for (const compound of compounds) {
		const rawName = getAllText(findChild(compound, "name")).trim();
		if (!rawName.startsWith("CppUtils::")) continue;
		if (isDetailNamespace(rawName)) continue;

		const refId = compound.attributes?.refid;
		if (!refId) continue;

		const kind = compound.attributes?.kind;
		const tokens = splitQualifiedName(rawName);
		const rawSymbolName = tokens.pop();
		const symbolName = cleanTemplateSpaces(rawSymbolName);
		tokens.shift(); // Remove "CppUtils"
		const subNamespace = tokens.join("/");
		const namespaceKey = tokens.join("::") || "Core";

		if (kind === "concept") {
			const dirPrefix = subNamespace ? `${subNamespace.toLowerCase()}/` : "";
			const url = `/${lang}/reference/${dirPrefix}concepts/#${sanitizeHtmlId(symbolName)}`;
			const compoundTarget = { url, name: symbolName };
			refMap.set(refId, compoundTarget);
			if (!symbolCatalog.has(symbolName)) symbolCatalog.set(symbolName, compoundTarget);
			if (!symbolCatalog.has(rawSymbolName)) symbolCatalog.set(rawSymbolName, compoundTarget);
			if (namespaceKey && !symbolCatalog.has(`${namespaceKey}::${symbolName}`)) {
				symbolCatalog.set(`${namespaceKey}::${symbolName}`, compoundTarget);
			}
			const cleanRaw = rawName.replace(/\s+/g, "");
			if (!symbolCatalog.has(cleanRaw)) symbolCatalog.set(cleanRaw, compoundTarget);
			const cleanWithoutPrefix = rawName.replace(/^CppUtils::/, "").replace(/\s+/g, "");
			if (!symbolCatalog.has(cleanWithoutPrefix)) symbolCatalog.set(cleanWithoutPrefix, compoundTarget);
			continue;
		}

		let safeBaseName = sanitizeFileName(symbolName);
		const slugKey = `${subNamespace}/${safeBaseName}`.toLowerCase();
		let currentCount = slugCounts.get(slugKey) || 0;
		currentCount++;
		slugCounts.set(slugKey, currentCount);
		if (currentCount > 1) safeBaseName = `${safeBaseName}_${currentCount}`;

		const dirPrefix = subNamespace ? `${subNamespace.toLowerCase()}/` : "";
		const url = `/${lang}/reference/${dirPrefix}${safeBaseName.toLowerCase()}/`;
		const compoundTarget = { url, name: symbolName };
		refMap.set(refId, compoundTarget);
		if (!symbolCatalog.has(symbolName)) symbolCatalog.set(symbolName, compoundTarget);
		if (!symbolCatalog.has(rawSymbolName)) symbolCatalog.set(rawSymbolName, compoundTarget);
		if (namespaceKey && !symbolCatalog.has(`${namespaceKey}::${symbolName}`)) {
			symbolCatalog.set(`${namespaceKey}::${symbolName}`, compoundTarget);
		}
		const cleanRaw = rawName.replace(/\s+/g, "");
		if (!symbolCatalog.has(cleanRaw)) symbolCatalog.set(cleanRaw, compoundTarget);
		const cleanWithoutPrefix = rawName.replace(/^CppUtils::/, "").replace(/\s+/g, "");
		if (!symbolCatalog.has(cleanWithoutPrefix)) symbolCatalog.set(cleanWithoutPrefix, compoundTarget);

		// Index class/struct members into refMap and symbolCatalog for autolinking
		const seenMemberIds = new Set();
		for (const memberNode of findChildren(compound, "member")) {
			const memberRefId = memberNode.attributes?.refid;
			const memberName = cleanSignature(getAllText(findChild(memberNode, "name")));
			const memberKind = memberNode.attributes?.kind;
			if (!memberRefId || !memberName) continue;

			const isFunction = memberKind === "function";
			let anchor = isFunction ? sanitizeHtmlId(memberName) : sanitizeHtmlId(memberName).toLowerCase();
			if (seenMemberIds.has(anchor)) {
				let overloadCounter = 2;
				while (seenMemberIds.has(`${anchor}_${overloadCounter}`)) overloadCounter++;
				anchor = `${anchor}_${overloadCounter}`;
			}
			seenMemberIds.add(anchor);

			const memberTarget = { url: `${url}#${anchor}`, name: memberName };
			refMap.set(memberRefId, memberTarget);
			if (!symbolCatalog.has(`${symbolName}::${memberName}`)) {
				symbolCatalog.set(`${symbolName}::${memberName}`, memberTarget);
			}
			if (!symbolCatalog.has(`${rawSymbolName}::${memberName}`)) {
				symbolCatalog.set(`${rawSymbolName}::${memberName}`, memberTarget);
			}
			if (namespaceKey && !symbolCatalog.has(`${namespaceKey}::${symbolName}::${memberName}`)) {
				symbolCatalog.set(`${namespaceKey}::${symbolName}::${memberName}`, memberTarget);
			}
			const fullMemberSymbol = `CppUtils::${tokens.concat([symbolName, memberName]).join("::")}`;
			if (!symbolCatalog.has(fullMemberSymbol)) {
				symbolCatalog.set(fullMemberSymbol, memberTarget);
			}
		}

		// Index primary template signatures to cross-reference partial specializations
		if (!symbolName.includes("<")) {
			let primaryTemplateParams = "";
			const xmlPath = path.join(xmlDir, `${refId}.xml`);
			if (fs.existsSync(xmlPath)) {
				const fileContent = fs.readFileSync(xmlPath, "utf-8");
				const match = fileContent.match(/<templateparamlist>([\s\S]*?)<\/templateparamlist>/);
				if (match) {
					const paramTags = match[1].match(/<param>[\s\S]*?<\/param>/g) || [];
					const params = paramTags.map((p) => {
						const type = cleanSignature((p.match(/<type>([\s\S]*?)<\/type>/)?.[1] || "").replace(/<[^>]+>/g, ""));
						const declarationName = cleanSignature((p.match(/<declname>([\s\S]*?)<\/declname>/)?.[1] || "").replace(/<[^>]+>/g, ""));
						const defaultValue = cleanSignature((p.match(/<defval>([\s\S]*?)<\/defval>/)?.[1] || "").replace(/<[^>]+>/g, ""));
						let str = [type, declarationName].filter(Boolean).join(" ");
						if (defaultValue) str += ` = ${defaultValue}`;
						return str;
					});
					if (params.length > 0) primaryTemplateParams = `<${params.join(", ")}>`;
				}
			}
			primaryTemplates.set(`${namespaceKey}::${symbolName}`, {
				name: `CppUtils::${tokens.concat([symbolName]).join("::")}`,
				url,
				templateParams: primaryTemplateParams,
			});
		}
	}

	// Index free functions and namespaces into symbolCatalog and refMap
	for (const namespaceCompound of namespaceCompounds) {
		const rawName = getAllText(findChild(namespaceCompound, "name")).trim();
		if (!rawName.startsWith("CppUtils")) continue;
		if (isDetailNamespace(rawName)) continue;

		const tokens = splitQualifiedName(rawName);
		if (tokens[0] === "CppUtils") tokens.shift();
		const subNamespace = tokens.join("/");
		const namespaceKey = tokens.join("::") || "Core";
		const dirPrefix = subNamespace ? `${subNamespace.toLowerCase()}/` : "";
		const url = subNamespace ? `/${lang}/reference/${dirPrefix}functions/` : `/${lang}/reference/globalfunctions/`;

		// Index the namespace compound so cross-references to qualified sub-namespaces resolve to their functions page
		const namespaceRefId = namespaceCompound.attributes?.refid;
		if (namespaceRefId) {
			const namespaceTarget = { url, name: namespaceKey };
			refMap.set(namespaceRefId, namespaceTarget);
			if (!symbolCatalog.has(namespaceKey)) {
				symbolCatalog.set(namespaceKey, namespaceTarget);
			}
			const fullNamespaceSymbol = `CppUtils::${namespaceKey}`;
			if (!symbolCatalog.has(fullNamespaceSymbol)) {
				symbolCatalog.set(fullNamespaceSymbol, namespaceTarget);
			}
		}

		for (const memberNode of findChildren(namespaceCompound, "member")) {
			const memberRefId = memberNode.attributes?.refid;
			const memberName = cleanSignature(getAllText(findChild(memberNode, "name")));
			const memberKind = memberNode.attributes?.kind;
			if (!memberRefId || !memberName) continue;
			if (memberKind !== "function" && memberKind !== "enum" && memberKind !== "typedef" && memberKind !== "variable") continue;

			// Functions and variables use HTML divs with preserved casing in id.
			// Enums and types use markdown headings ### `Name` which Starlight lowercases in the anchor.
			const isMarkdownHeading = memberKind === "enum" || memberKind === "typedef";
			const anchor = isMarkdownHeading ? sanitizeHtmlId(memberName).toLowerCase() : sanitizeHtmlId(memberName);
			const memberTarget = { url: `${url}#${anchor}`, name: memberName };
			refMap.set(memberRefId, memberTarget);
			if (!symbolCatalog.has(memberName)) symbolCatalog.set(memberName, memberTarget);
			if (namespaceKey && !symbolCatalog.has(`${namespaceKey}::${memberName}`)) {
				symbolCatalog.set(`${namespaceKey}::${memberName}`, memberTarget);
			}
			const fullMemberSymbol = `CppUtils::${tokens.concat([memberName]).join("::")}`;
			if (!symbolCatalog.has(fullMemberSymbol)) {
				symbolCatalog.set(fullMemberSymbol, memberTarget);
			}

			// Index user-defined literal suffixes without the operator prefix for direct symbol resolution
			if (memberName.startsWith('operator""')) {
				const literalSuffix = memberName.slice(10).trim();
				if (literalSuffix) {
					if (!symbolCatalog.has(literalSuffix) || rawName.includes("Literals")) {
						symbolCatalog.set(literalSuffix, memberTarget);
					}
					if (namespaceKey && !symbolCatalog.has(`${namespaceKey}::${literalSuffix}`)) {
						symbolCatalog.set(`${namespaceKey}::${literalSuffix}`, memberTarget);
					}
				}
			}
		}
	}

	return { refMap, symbolCatalog, primaryTemplates };
}

function getBadgeInfo(kind) {
	switch (kind?.toLowerCase()) {
		case "class":
			return { badgeText: "CLASS", badgeClass: "ref-badge ref-badge-class" };
		case "struct":
			return { badgeText: "STRUCT", badgeClass: "ref-badge ref-badge-struct" };
		case "concept":
		case "concepts":
			return { badgeText: "CONCEPTS", badgeClass: "ref-badge ref-badge-concept" };
		case "functions":
			return { badgeText: "FUNCTIONS", badgeClass: "ref-badge ref-badge-functions" };
		case "enum":
			return { badgeText: "ENUM", badgeClass: "ref-badge ref-badge-enum" };
		case "constants":
			return { badgeText: "CONST", badgeClass: "ref-badge ref-badge-constants" };
		case "types":
			return { badgeText: "TYPE", badgeClass: "ref-badge ref-badge-types" };
		default:
			return { badgeText: (kind || "").toUpperCase(), badgeClass: "ref-badge" };
	}
}

async function generateForLanguage(lang, compounds, namespaceCompounds = []) {
	const outDir = path.join(docsRoot, "src", "content", "docs", lang, "reference");
	if (fs.existsSync(outDir)) fs.rmSync(outDir, { recursive: true, force: true });
	fs.mkdirSync(outDir, { recursive: true });

	const isFr = lang === "fr";
	const labels = {
		declaration: isFr ? "Déclaration" : "Declaration",
		description: isFr ? "Description" : "Description",
		publicTypes: isFr ? "Types publics" : "Public types",
		publicMethods: isFr ? "Méthodes publiques" : "Public methods",
		publicFuncs: isFr ? "Fonctions publiques" : "Public functions",
		publicAttrs: isFr ? "Attributs publics" : "Public attributes",
		publicEnums: isFr ? "Énumérations publiques" : "Public enums",
		publicConstants: isFr ? "Constantes & variables" : "Constants & variables",
		namespaceFunctions: isFr ? "Fonctions & utilitaires" : "Functions & utilities",
		constants: isFr ? "Constantes" : "Constants",
		enums: isFr ? "Énumérations" : "Enums",
		types: isFr ? "Définitions de types" : "Type definitions",
		namespace: "Namespace:",
		fullSymbol: isFr ? "Symbole complet :" : "Full symbol:",
		apiRefTitle: isFr ? "Référence d'API CppUtils" : "CppUtils API reference",
		apiRefDesc: isFr ? "Catalogue complet des modules et composants de CppUtils" : "Complete catalog of CppUtils modules and components",
		publicConcepts: isFr ? "Concepts publics" : "Public concepts",
		concepts: "Concepts",
	};

	const { refMap, symbolCatalog, primaryTemplates } = buildSymbolCatalog(compounds, namespaceCompounds, lang);
	const namespacesMap = new Map();
	const conceptsByNamespace = new Map();
	const usedSlugs = new Set();
	let generatedCount = 0;

	for (const compound of compounds) {
		const kind = compound.attributes.kind;
		const rawName = getAllText(findChild(compound, "name")).trim();
		if (!rawName.startsWith("CppUtils::")) continue;
		if (isDetailNamespace(rawName)) continue;

		const refId = compound.attributes.refid;
		const xmlPath = path.join(xmlDir, `${refId}.xml`);
		if (!fs.existsSync(xmlPath)) continue;

		const fileContent = fs.readFileSync(xmlPath, "utf-8");
		const fileTree = await parseXml(fileContent);
		const compounddef = findChild(fileTree, "compounddef");
		if (!compounddef) continue;

		const compoundFile = findChild(compounddef, "location")?.attributes?.file;

		const tokens = splitQualifiedName(rawName);
		const rawSymbolName = tokens.pop();
		const symbolName = cleanTemplateSpaces(rawSymbolName);
		tokens.shift(); // Remove "CppUtils"
		const subNamespace = tokens.join("/");
		const namespaceKey = tokens.join("::") || "Core";

		if (kind === "concept") {
			if (!conceptsByNamespace.has(namespaceKey)) {
				conceptsByNamespace.set(namespaceKey, {
					tokens: [...tokens],
					subNamespace,
					fullNamespaceName: `CppUtils${tokens.length > 0 ? "::" + tokens.join("::") : ""}`,
					concepts: [],
				});
			}
			const briefDesc = renderDocNode(findChild(compounddef, "briefdescription"), lang, refMap, symbolCatalog).trim();
			const detailedDesc = renderDocNode(findChild(compounddef, "detaileddescription"), lang, refMap, symbolCatalog).trim();
			conceptsByNamespace.get(namespaceKey).concepts.push({
				compounddef,
				symbolName,
				rawName,
				briefDesc,
				detailedDesc,
				compoundFile,
			});
			if (lang === "en") {
				const cleanFullName = rawName.replace(/\s+/g, "");
				cpputilsSymbols[cleanFullName] = `/reference/${subNamespace ? `${subNamespace.toLowerCase()}/` : ""}concepts/#${sanitizeHtmlId(symbolName)}`;
			}
			continue;
		}

		const targetDir = subNamespace ? path.join(outDir, subNamespace) : outDir;
		fs.mkdirSync(targetDir, { recursive: true });

		let safeBaseName = sanitizeFileName(symbolName);
		const slugKey = `${subNamespace}/${safeBaseName}`.toLowerCase();
		if (usedSlugs.has(slugKey)) {
			let overloadCounter = 2;
			while (usedSlugs.has(`${slugKey}_${overloadCounter}`)) overloadCounter++;
			safeBaseName = `${safeBaseName}_${overloadCounter}`;
			usedSlugs.add(`${slugKey}_${overloadCounter}`);
		} else usedSlugs.add(slugKey);

		const fileName = `${safeBaseName}.mdx`;
		const filePath = path.join(targetDir, fileName);

		const briefDesc = renderDocNode(findChild(compounddef, "briefdescription"), lang, refMap, symbolCatalog).trim();
		const detailedDesc = renderDocNode(findChild(compounddef, "detaileddescription"), lang, refMap, symbolCatalog).trim();
		const templateHeader = extractTemplateParams(compounddef);

		const pageDesc = isFr
			? `Référence d'API pour CppUtils::${tokens.concat([symbolName]).join("::")}`
			: `API Reference for CppUtils::${tokens.concat([symbolName]).join("::")}`;

		const nsPrefix = `CppUtils::${tokens.length > 0 ? tokens.join("::") + "::" : ""}`;
		// Extract C++20 module name if declared in the source file
		const moduleName = extractModuleFromSource(compoundFile) || (tokens.length > 0 ? `CppUtils.${tokens.join(".")}` : "CppUtils");
		const templateBrackets = templateHeader ? templateHeader.replace(/^\s*template\s*/, "").trim() : "";

		// Detect partial or full template specializations (e.g. MyType<T*>)
		const specMatch = symbolName.match(/^([^<]+)(<.+>)$/);
		const baseSymbolName = specMatch ? specMatch[1].trim() : symbolName;
		const specializationArgs = specMatch ? specMatch[2].trim() : "";

		const symbolSlug = `/reference/${subNamespace ? `${subNamespace}/` : ""}${safeBaseName}/`.toLowerCase();
		if (lang === "en") {
			const cleanFullName = rawName.replace(/\s+/g, "");
			cpputilsSymbols[cleanFullName] = symbolSlug;
			if (baseSymbolName !== symbolName) {
				const baseFullName = `CppUtils::${tokens.concat([baseSymbolName]).join("::")}`;
				if (!cpputilsSymbols[baseFullName]) {
					cpputilsSymbols[baseFullName] = symbolSlug;
				}
			}
		}

		const displayTitle = baseSymbolName;
		const displayTemplate = specializationArgs || templateBrackets;

		let markdown = `---\ntitle: "${displayTitle}"\n`;
		if (specializationArgs) markdown += `sidebar:\n  label: "${symbolName.replace(/"/g, '\\"')}"\n`;
		markdown += `description: "${pageDesc}"\nnamespace: "${nsPrefix}"\n`;
		if (displayTemplate) markdown += `templateParams: "${displayTemplate.replace(/"/g, '\\"')}"\n`;
		if (moduleName) markdown += `moduleImport: "${moduleName}"\n`;
		markdown += `badge: "${kind.toUpperCase()}"\n`;

		if (specMatch) {
			const primary = primaryTemplates.get(`${namespaceKey}::${baseSymbolName}`);
			if (primary) {
				markdown += `specializationOf:\n  name: "${primary.name}"\n`;
				if (primary.url) markdown += `  url: "${primary.url}"\n`;
				if (primary.templateParams) markdown += `  templateParams: "${primary.templateParams.replace(/"/g, '\\"')}"\n`;
			} else {
				const fullBase = `CppUtils::${tokens.concat([baseSymbolName]).join("::")}`;
				markdown += `specializationOf:\n  name: "${fullBase}"\n`;
			}
		}
		markdown += `---\n\n`;

		if (briefDesc) markdown += `${briefDesc}\n\n`;

		if (detailedDesc) markdown += `## ${labels.description}\n\n${detailedDesc}\n\n`;

		// Collect public members grouped by category (methods, types, attributes)
		const sections = findChildren(compounddef, "sectiondef");
		const publicFuncs = [];
		const publicTypes = [];
		const publicAttrs = [];

		for (const section of sections) {
			const sectionKind = section.attributes.kind;
			const members = findChildren(section, "memberdef");
			for (const memberNode of members) {
				const prot = memberNode.attributes.prot;
				if (prot !== "public") continue;

				if (sectionKind.includes("func")) publicFuncs.push(memberNode);
				else if (sectionKind.includes("type")) publicTypes.push(memberNode);
				else if (sectionKind.includes("attrib")) publicAttrs.push(memberNode);
			}
		}

		const seenMemberIds = new Set();

		if (publicTypes.length > 0) {
			markdown += `## ${labels.publicTypes}\n\n`;
			for (const typeNode of publicTypes) {
				const typeName = cleanSignature(getAllText(findChild(typeNode, "name")));
				const typeDefinition = cleanTemplateSpaces(cleanSignature(getAllText(findChild(typeNode, "definition"))));
				const typeBrief = renderDocNode(findChild(typeNode, "briefdescription"), lang, refMap, symbolCatalog).trim();
				markdown += renderMemberCard(null, typeName, cleanTemplateSpaces(`${typeDefinition || typeName};`), typeBrief, "");
			}
		}

		if (publicAttrs.length > 0) {
			markdown += `## ${labels.publicAttrs}\n\n`;
			for (const attributeNode of publicAttrs) {
				const attributeName = cleanSignature(getAllText(findChild(attributeNode, "name")));
				const attributeType = cleanCppType(getAllText(findChild(attributeNode, "type")));
				const attributeInitializer = cleanSignature(getAllText(findChild(attributeNode, "initializer")));
				const attributeBrief = renderDocNode(findChild(attributeNode, "briefdescription"), lang, refMap, symbolCatalog).trim();
				const attributeDetailed = renderDocNode(findChild(attributeNode, "detaileddescription"), lang, refMap, symbolCatalog).trim();

				let isStatic = attributeNode.attributes?.static === "yes";
				let isConstexpr = attributeNode.attributes?.constexpr === "yes";
				let prefix = "";
				if (isStatic) prefix += "static ";
				if (isConstexpr) prefix += "constexpr ";

				let cleanId = sanitizeHtmlId(attributeName) || "attr";
				if (seenMemberIds.has(cleanId)) {
					let counter = 2;
					while (seenMemberIds.has(`${cleanId}_${counter}`)) counter++;
					cleanId = `${cleanId}_${counter}`;
				}
				seenMemberIds.add(cleanId);

				if (lang === "en") {
					const cleanFullName = rawName.replace(/\s+/g, "");
					cpputilsSymbols[`${cleanFullName}::${attributeName}`] = `${symbolSlug}#${cleanId}`;
				}

				const anchorLabel = lang === "fr" ? `Lien direct vers ${cleanId}` : `Direct link to ${cleanId}`;
				const codeBlock = cleanTemplateSpaces(`${prefix}${attributeType} ${attributeName}${attributeInitializer ? " " + attributeInitializer : ""};`);
				markdown += renderMemberCard(cleanId, null, codeBlock, attributeBrief, attributeDetailed, anchorLabel);
			}
		}

		if (publicFuncs.length > 0) {
			markdown += `## ${labels.publicMethods}\n\n`;
			for (const functionNode of publicFuncs) {
				const functionName = cleanSignature(getAllText(findChild(functionNode, "name")));
				const functionTemplate = extractTemplateParams(functionNode);
				const functionBrief = renderDocNode(findChild(functionNode, "briefdescription"), lang, refMap, symbolCatalog).trim();
				const functionDetailed = renderDocNode(findChild(functionNode, "detaileddescription"), lang, refMap, symbolCatalog).trim();
				const functionSignature = formatFunctionSignature(functionNode, baseSymbolName);

				let cleanId = sanitizeHtmlId(functionName) || "method";
				if (seenMemberIds.has(cleanId)) {
					let overloadCounter = 2;
					while (seenMemberIds.has(`${cleanId}_${overloadCounter}`)) overloadCounter++;
					cleanId = `${cleanId}_${overloadCounter}`;
				}
				seenMemberIds.add(cleanId);

				if (lang === "en") {
					const cleanFullName = rawName.replace(/\s+/g, "");
					cpputilsSymbols[`${cleanFullName}::${functionName}`] = `${symbolSlug}#${cleanId}`;
				}

				const anchorLabel = lang === "fr" ? `Lien direct vers ${cleanId}` : `Direct link to ${cleanId}`;
				const codeBlock = functionTemplate && !functionSignature.startsWith("template") ? `${functionTemplate}\n${functionSignature}` : functionSignature;
				markdown += renderMemberCard(cleanId, null, codeBlock, functionBrief, functionDetailed, anchorLabel);
			}
		}

		fs.writeFileSync(filePath, markdown, "utf-8");
		generatedCount++;

		if (!namespacesMap.has(namespaceKey)) namespacesMap.set(namespaceKey, []);
		namespacesMap.get(namespaceKey).push({ name: symbolName, kind, file: fileName, subNamespace });
	}

	// Generate Concepts.mdx for each namespace containing exported concepts
	for (const [namespaceKey, data] of conceptsByNamespace) {
		const { tokens, subNamespace, fullNamespaceName, concepts } = data;
		if (concepts.length === 0) continue;

		concepts.sort((a, b) => a.symbolName.localeCompare(b.symbolName));

		const targetDir = subNamespace ? path.join(outDir, subNamespace) : outDir;
		fs.mkdirSync(targetDir, { recursive: true });
		const filePath = path.join(targetDir, "Concepts.mdx");

		const title = labels.concepts;
		const pageDesc = isFr
			? `Concepts publics du namespace ${fullNamespaceName}`
			: `Public concepts for namespace ${fullNamespaceName}`;

		const firstFile = concepts[0]?.compoundFile;
		const moduleName = extractModuleFromSource(firstFile) || (tokens.length > 0 ? `CppUtils.${tokens.join(".")}` : "CppUtils");

		let markdown = `---\ntitle: "${title}"\ndescription: "${pageDesc}"\nbadge: "CONCEPTS"\nmoduleImport: "${moduleName}"\nsidebar:\n  order: 2\n---\n\n`;
		markdown += `**${labels.namespace}** \`${fullNamespaceName}\`\n\n`;
		markdown += `## ${labels.publicConcepts}\n\n`;

		for (const concept of concepts) {
			const anchorId = sanitizeHtmlId(concept.symbolName);
			const anchorLabel = isFr ? `Lien direct vers ${concept.symbolName}` : `Direct link to ${concept.symbolName}`;
			const codeBlock = formatConceptDeclaration(concept.compounddef, concept.symbolName);
			markdown += renderMemberCard(anchorId, null, codeBlock, concept.briefDesc, concept.detailedDesc, anchorLabel);
		}

		fs.writeFileSync(filePath, markdown, "utf-8");
		generatedCount++;

		if (!namespacesMap.has(namespaceKey)) namespacesMap.set(namespaceKey, []);
		namespacesMap.get(namespaceKey).push({ name: labels.concepts, kind: "concepts", file: "Concepts.mdx", subNamespace });
	}

	// Process namespace-level compounds (free functions, enums, constants, typedefs)
	for (const namespaceCompound of namespaceCompounds || []) {
		const rawName = getAllText(findChild(namespaceCompound, "name")).trim();
		if (!rawName.startsWith("CppUtils")) continue;
		if (isDetailNamespace(rawName)) continue;

		const refId = namespaceCompound.attributes.refid;
		const xmlPath = path.join(xmlDir, `${refId}.xml`);
		if (!fs.existsSync(xmlPath)) continue;

		const fileContent = fs.readFileSync(xmlPath, "utf-8");
		const fileTree = await parseXml(fileContent);
		const compounddef = findChild(fileTree, "compounddef");
		if (!compounddef) continue;

		const sections = findChildren(compounddef, "sectiondef");
		const publicFuncs = [];
		const publicEnums = [];
		const publicTypes = [];
		const publicVars = [];
		const seenIds = new Set();

		for (const section of sections) {
			const sectionKind = section.attributes.kind;
			const members = findChildren(section, "memberdef");
			for (const memberNode of members) {
				const prot = memberNode.attributes.prot || "public";
				if (prot !== "public") continue;
				const memberId = memberNode.attributes.id;
				if (seenIds.has(memberId)) continue;
				seenIds.add(memberId);

				if (sectionKind.includes("func")) publicFuncs.push(memberNode);
				else if (sectionKind.includes("enum")) publicEnums.push(memberNode);
				else if (sectionKind.includes("typedef")) publicTypes.push(memberNode);
				else if (sectionKind.includes("var")) publicVars.push(memberNode);
			}
		}

		if (publicFuncs.length === 0 && publicEnums.length === 0 && publicTypes.length === 0 && publicVars.length === 0) continue;

		const tokens = splitQualifiedName(rawName);
		if (tokens[0] === "CppUtils") tokens.shift();
		const subNamespace = tokens.join("/");
		const namespaceKey = tokens.join("::") || "Core";

		let fileName = "Functions.mdx";
		let title = labels.namespaceFunctions;
		let badgeText = "FUNCTIONS";
		let itemKind = "functions";
		let sidebarOrder = 1;

		if (publicFuncs.length > 0) {
			fileName = "Functions.mdx";
			title = labels.namespaceFunctions;
			badgeText = "FUNCTIONS";
			itemKind = "functions";
			sidebarOrder = 1;
		} else if (publicVars.length > 0) {
			fileName = "Constants.mdx";
			title = labels.constants;
			badgeText = "CONSTANTS";
			itemKind = "constants";
			sidebarOrder = 3;
		} else if (publicEnums.length > 0) {
			fileName = "Enums.mdx";
			title = labels.enums;
			badgeText = "ENUM";
			itemKind = "enum";
			sidebarOrder = 4;
		} else {
			fileName = "Types.mdx";
			title = labels.types;
			badgeText = "TYPES";
			itemKind = "types";
			sidebarOrder = 5;
		}

		if (!subNamespace) {
			fileName = "GlobalFunctions.mdx";
			title = isFr ? "Fonctions globales CppUtils" : "Global CppUtils Functions";
			sidebarOrder = 1;
		}

		const targetDir = subNamespace ? path.join(outDir, subNamespace) : outDir;
		fs.mkdirSync(targetDir, { recursive: true });
		const filePath = path.join(targetDir, fileName);

		const fullNamespaceName = rawName;
		const pageDesc = isFr
			? `${title} du namespace ${fullNamespaceName}`
			: `${title} for namespace ${fullNamespaceName}`;

		const nsModule = fullNamespaceName.replace(/::/g, ".");
		const sidebarSection = tokens.length <= 1 ? `sidebar:\n  order: ${sidebarOrder}\n` : "";
		let markdown = `---\ntitle: "${title}"\ndescription: "${pageDesc}"\nbadge: "${badgeText}"\nmoduleImport: "${nsModule}"\n${sidebarSection}---\n\n`;
		markdown += `**${labels.namespace}** \`${fullNamespaceName}\`\n\n`;
		const nsBrief = renderDocNode(findChild(compounddef, "briefdescription"), lang, refMap, symbolCatalog).trim();
		const nsDetailed = renderDocNode(findChild(compounddef, "detaileddescription"), lang, refMap, symbolCatalog).trim();
		if (nsBrief) markdown += `${nsBrief}\n\n`;
		if (nsDetailed) markdown += `${nsDetailed}\n\n`;
		markdown += renderNamespaceContent(publicEnums, publicTypes, publicFuncs, publicVars, labels, lang, refMap, symbolCatalog);

		fs.writeFileSync(filePath, markdown, "utf-8");
		generatedCount++;

		if (lang === "en") {
			const baseNamespaceSlug = `/reference/${subNamespace ? `${subNamespace.toLowerCase()}/` : ""}${fileName.replace(/\.mdx$/, "").toLowerCase()}/`;
			for (const functionNode of publicFuncs) {
				const functionName = cleanSignature(getAllText(findChild(functionNode, "name")));
				const fullSymbol = `${fullNamespaceName}::${functionName}`;
				if (!cpputilsSymbols[fullSymbol]) {
					cpputilsSymbols[fullSymbol] = `${baseNamespaceSlug}#${sanitizeHtmlId(functionName)}`;
				}
			}
			for (const enumNode of publicEnums) {
				const enumName = cleanSignature(getAllText(findChild(enumNode, "name")));
				const fullSymbol = `${fullNamespaceName}::${enumName}`;
				if (!cpputilsSymbols[fullSymbol]) {
					cpputilsSymbols[fullSymbol] = `${baseNamespaceSlug}#${sanitizeHtmlId(enumName).toLowerCase()}`;
				}
			}
			for (const variableNode of publicVars) {
				const variableName = cleanSignature(getAllText(findChild(variableNode, "name")));
				const fullSymbol = `${fullNamespaceName}::${variableName}`;
				if (!cpputilsSymbols[fullSymbol]) {
					cpputilsSymbols[fullSymbol] = `${baseNamespaceSlug}#${sanitizeHtmlId(variableName)}`;
				}
			}
			for (const typeNode of publicTypes) {
				const typeName = cleanSignature(getAllText(findChild(typeNode, "name")));
				const fullSymbol = `${fullNamespaceName}::${typeName}`;
				if (!cpputilsSymbols[fullSymbol]) {
					cpputilsSymbols[fullSymbol] = `${baseNamespaceSlug}#${sanitizeHtmlId(typeName).toLowerCase()}`;
				}
			}
		}

		if (!namespacesMap.has(namespaceKey)) namespacesMap.set(namespaceKey, []);
		namespacesMap.get(namespaceKey).push({ name: title, kind: itemKind, file: fileName, subNamespace });
	}

	// Create Reference Overview Index page
	let indexMd = `---\ntitle: "${labels.apiRefTitle}"\ndescription: "${labels.apiRefDesc}"\n---\n\n`;
	indexMd += `export const base = import.meta.env.BASE_URL.replace(/\\/$/, '');\n\n`;

	const sortedNamespaces = [...namespacesMap.keys()].sort();
	indexMd += `<div class="reference-container">\n`;
	indexMd += `  <div class="reference-masonry">\n`;
	for (const namespaceName of sortedNamespaces) {
		const items = namespacesMap.get(namespaceName).sort((a, b) => a.name.localeCompare(b.name));
		indexMd += `    <div class="reference-category-card">\n`;
		indexMd += `      <div class="reference-card-header">\n`;
		indexMd += `        <h3 class="reference-card-title"><code>CppUtils::${namespaceName}</code></h3>\n`;
		indexMd += `      </div>\n`;
		indexMd += `      <ul class="reference-card-list">\n`;
		for (const item of items) {
			const dirPrefix = item.subNamespace ? `${item.subNamespace.toLowerCase()}/` : "";
			const baseSlug = item.file.replace(/\.mdx?$/, "").toLowerCase();
			const link = `/${lang}/reference/${dirPrefix}${baseSlug}/`;
			const { badgeText, badgeClass } = getBadgeInfo(item.kind);
			indexMd += `        <li class="reference-item">\n`;
			indexMd += `          <a href={\`\${base}${link}\`} class="reference-item-link">\n`;
			indexMd += `            <code class="reference-item-name">{${JSON.stringify(item.name)}}</code>\n`;
			indexMd += `            <span class="${badgeClass}">${badgeText}</span>\n`;
			indexMd += `          </a>\n`;
			indexMd += `        </li>\n`;
		}
		indexMd += `      </ul>\n`;
		indexMd += `    </div>\n\n`;
	}
	indexMd += `  </div>\n`;
	indexMd += `</div>\n`;

	fs.writeFileSync(path.join(outDir, "index.mdx"), indexMd, "utf-8");
	console.log(`[generate-api] [${lang.toUpperCase()}] Successfully generated ${generatedCount} API reference pages across ${sortedNamespaces.length} namespaces.`);
}

async function main() {
	await updateCppReferenceSymbols({ maxAgeDays: 7, timeoutMs: 3000 });
	console.log("[generate-api] Scanning Doxygen XML files...");

	const indexXmlContent = fs.readFileSync(path.join(xmlDir, "index.xml"), "utf-8");
	const indexTree = await parseXml(indexXmlContent);

	const allCompounds = findChildren(indexTree, "compound");
	const validKinds = new Set(["class", "struct", "concept"]);
	const targetCompounds = allCompounds.filter((c) => validKinds.has(c.attributes.kind));
	const namespaceCompounds = allCompounds.filter((c) => c.attributes.kind === "namespace");

	// Generate symmetrically for English and French
	await generateForLanguage("en", targetCompounds, namespaceCompounds);
	await generateForLanguage("fr", targetCompounds, namespaceCompounds);

	const symbolsOutPath = path.join(docsRoot, "src", "data", "cpputils-symbols.json");
	fs.writeFileSync(symbolsOutPath, JSON.stringify(cpputilsSymbols, null, 2), "utf-8");
	console.log(`[generate-api] Saved ${Object.keys(cpputilsSymbols).length} CppUtils symbols to ${symbolsOutPath}`);
}

main().catch((err) => {
	console.error("[generate-api] Fatal error:", err);
	process.exit(1);
});
