import fs from "node:fs";
import path from "node:path";
import { fileURLToPath } from "node:url";
import sax from "sax";
import { updateCppReferenceSymbols } from "./update-cppreference-symbols.mjs";

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const docsRoot = path.resolve(__dirname, "..");
const xmlDir = path.join(docsRoot, "doxygen_xml");

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
			const el = { tag: node.name, attributes: node.attributes || {}, children: [], text: "" };
			stack[stack.length - 1].children.push(el);
			stack.push(el);
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

function renderDocNode(node, lang = "en", refMap = null) {
	if (!node) return "";
	if (node.tag === "#text") return node.text || "";
	let result = "";

	for (const child of node.children || []) {
		if (child.tag === "#text") result += child.text || "";
		else if (child.tag === "para") {
			const paraText = renderDocNode(child, lang, refMap).trim();
			if (paraText) result += `${paraText}\n\n`;
		} else if (child.tag === "parameterlist") {
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
					const descNode = findChild(findChild(item, "parameterdescription"), "para");
					const name = getAllText(nameNode).trim();
					const desc = renderDocNode(descNode, lang, refMap).trim().replace(/\n+/g, " ");
					if (name) result += `| \`${name}\` | ${desc} |\n`;
				}
				result += "\n";
			}
		} else if (child.tag === "simplesect") {
			const kind = child.attributes.kind;
			if (kind === "see") {
				const paras = findChildren(child, "para");
				const targetParas = paras.length > 0 ? paras : [child];
				for (const para of targetParas) {
					const allParaText = getAllText(para).trim();
					const ulink = findChild(para, "ulink") || findChildren(para, "para").map((p) => findChild(p, "ulink")).find(Boolean);
					const rawUrl = ulink?.attributes?.url || (allParaText.match(/https?:\/\/[^\s)]+/)?.[0] || "");

					const docMatch = rawUrl.match(/(?:https?:\/\/[^/]+)?\/?(?:CppUtils)?\/(fr|en)\/([^)\s]+)/);
					if (docMatch) {
						const linkLang = docMatch[1];
						const docPath = docMatch[2].replace(/\/+$/, "");
						if (linkLang !== lang) continue;

						const labelMatch = allParaText.match(/\(([^)]+)\)/);
						const label = labelMatch ? labelMatch[1].trim() : (lang === "fr" ? "Guide associé" : "Related guide");
						const targetUrl = `/${lang}/${docPath}/`;

						const noteTitle = lang === "fr" ? "Guide associé" : "Related guide";
						const noteText = lang === "fr"
							? `Consultez le [${label}](${targetUrl}) pour une explication détaillée et des exemples d'utilisation.`
							: `Check out the [${label}](${targetUrl}) for detailed explanations and usage examples.`;

						result += `\n:::note[${noteTitle}]\n${noteText}\n:::\n\n`;
						continue;
					}

					const content = renderDocNode(para, lang, refMap).trim();
					if (content) {
						result += `${lang === "fr" ? "\n**Voir aussi :** " : "\n**See also:** "}${content}\n\n`;
					}
				}
				continue;
			}
			const content = renderDocNode(child, lang, refMap).trim();
			if (kind === "return") result += `${lang === "fr" ? "\n**Retourne :** " : "\n**Returns:** "}${content}\n\n`;
			else if (kind === "note") result += `\n:::note\n${content}\n:::\n\n`;
			else if (kind === "tip" || kind === "remark" || kind === "remarks") result += `\n:::tip\n${content}\n:::\n\n`;
			else if (kind === "warning" || kind === "attention") result += `\n:::caution\n${content}\n:::\n\n`;
			else if (kind === "danger" || kind === "error") result += `\n:::danger\n${content}\n:::\n\n`;
			else result += `${content}\n\n`;
		} else if (child.tag === "ulink") {
			const url = child.attributes?.url || "";
			const text = renderDocNode(child, lang, refMap).trim() || url;
			result += `[${text}](${url})`;
		} else if (child.tag === "ref") {
			const text = getAllText(child).trim();
			const refId = child.attributes?.refid;
			let target = refMap?.get(refId);
			if (!target && refId) {
				const parentRefId = refId.replace(/_1[a-zA-Z0-9]+$/, "");
				target = refMap?.get(parentRefId);
			}

			if (target) result += `[\`${text}\`](${target.url})`;
			else if (child.attributes?.kindref === "compound" && refId?.startsWith("namespace")) result += text;
			else result += `\`${text}\``;
		} else if (child.tag === "computeroutput") result += `\`${getAllText(child).trim()}\``;
		else if (child.tag === "programlisting") {
			const codeLines = findChildren(child, "codeline").map((lineNode) => getAllText(lineNode));
			const code = codeLines.length > 0 ? codeLines.join("\n") : getAllText(child);
			result += `\n\`\`\`cpp showLineNumbers\n${code.trim()}\n\`\`\`\n\n`;
		} else result += renderDocNode(child, lang, refMap);
	}

	return decodeEntities(result);
}

function extractTemplateParams(node) {
	const tplNode = findChild(node, "templateparamlist");
	if (!tplNode) return "";
	const params = findChildren(tplNode, "param").map((p) => {
		const type = cleanSignature(getAllText(findChild(p, "type")));
		const declname = cleanSignature(getAllText(findChild(p, "declname")));
		const defval = cleanSignature(getAllText(findChild(p, "defval")));
		let str = [type, declname].filter(Boolean).join(" ");
		if (defval) str += ` = ${defval}`;
		return str;
	});
	return params.length > 0 ? `template <${params.join(", ")}>` : "";
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

function cleanTemplateSpaces(str) {
	if (!str) return "";
	let res = str;
	let prev;
	do {
		prev = res;
		res = res
			.replace(/<\s+/g, "<")
			.replace(/\s+>/g, ">")
			.replace(/>\s+>/g, ">>")
			.replace(/\s*,\s*/g, ", ");
	} while (res !== prev);
	return res;
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

function extractModuleFromSource(filePath) {
	if (!filePath) return null;
	const lines = getSourceLines(filePath);
	for (let i = 0; i < Math.min(lines.length, 250); i++) {
		const match = /export\s+module\s+([^;]+);/.exec(lines[i]);
		if (match) return match[1].trim();
	}
	return null;
}

function cleanCppType(type) {
	if (!type) return "";
	return cleanTemplateSpaces(decodeEntities(type))
		.replace(/\s+/g, " ")
		.replace(/(\bconst\s+[\w:]+|[\w:]+)\s+([&*]+)/g, "$1$2")
		.trim();
}

function cleanCppArgs(args) {
	if (!args) return "()";
	return cleanTemplateSpaces(decodeEntities(args))
		.replace(/\s+/g, " ")
		.replace(/\s*=\s*(delete|default|0)\b/g, " = $1")
		.replace(/(\bconst\s+[\w:]+|[\w:]+)\s+([&*]+)\s*(?:\.\.\.\s*)?(\w+)/g, (match, type, ref, name) => {
			const hasEllipsis = match.includes("...");
			return `${type}${ref}${hasEllipsis ? "... " : " "}${name}`;
		})
		.replace(/(\bconst\s+[\w:]+|[\w:]+)\s+([&*]+)\s*(?:\.\.\.\s*)?([,)])/g, (match, type, ref, end) => {
			const hasEllipsis = match.includes("...");
			return `${type}${ref}${hasEllipsis ? "..." : ""}${end}`;
		})
		.trim();
}

function findDeclarationEnd(buffer) {
	let inString = false;
	let inChar = false;
	let depthParen = 0;
	let depthBrace = 0;
	let seenParen = false;

	for (let i = 0; i < buffer.length; i++) {
		const ch = buffer[i];

		// Escape handling for strings and chars
		if (inString || inChar) {
			if (ch === "\\") {
				i++;
				continue;
			}
			if (inString && ch === '"') inString = false;
			else if (inChar && ch === "'") inChar = false;
			continue;
		}

		if (ch === '"') {
			inString = true;
			continue;
		}
		if (ch === "'") {
			// Check for C++ digit separator (1'000 or 0xFF'FF)
			if (i > 0 && /[\da-fA-F]/.test(buffer[i - 1]) && i + 1 < buffer.length && /[\da-fA-F]/.test(buffer[i + 1])) continue;
			inChar = true;
			continue;
		}

		// Skip single-line comments // ...
		if (ch === "/" && buffer[i + 1] === "/") {
			const newlineIdx = buffer.indexOf("\n", i);
			if (newlineIdx === -1) break;
			i = newlineIdx;
			continue;
		}

		if (ch === "(") {
			depthParen++;
			seenParen = true;
		} else if (ch === ")") {
			if (depthParen > 0) depthParen--;
		} else if (ch === "{") {
			if (depthParen === 0 && depthBrace === 0) {
				// Check if this { belongs to a requires expression (requires requires { ... })
				const prefix = buffer.slice(0, i).trimEnd();
				if (/\brequires(\s*\([^)]*\))?$/.test(prefix)) depthBrace++;
				else return i; // Function body begins here!
			} else depthBrace++;
		} else if (ch === "}") {
			if (depthBrace > 0) depthBrace--;
		} else if (ch === ";" && depthParen === 0 && depthBrace === 0) {
			// End of declaration without body (or = default / = delete)
			return i;
		} else if (
			ch === ":" &&
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

function extractDeclarationFromSource(filePath, startLine, className, memberName) {
	if (!filePath || !fs.existsSync(filePath)) return null;
	const lines = getSourceLines(filePath);
	if (!lines || startLine < 1 || startLine > lines.length) return null;

	let idx = startLine - 1;
	let buffer = "";
	let foundName = false;

	for (let i = idx; i < Math.min(lines.length, idx + 12); i++) {
		const rawLine = lines[i];
		buffer += (buffer ? " " : "") + rawLine.trim();

		if (memberName.startsWith("operator")) {
			if (buffer.includes("operator")) foundName = true;
		} else if (memberName.startsWith("~")) {
			if (buffer.includes("~")) foundName = true;
		} else {
			const esc = memberName.replace(/[-[\]{}()*+?.,\\^$|#\s]/g, "\\$&");
			if (new RegExp(`\\b${esc}\\b`).test(buffer)) foundName = true;
		}

		if (foundName) {
			const endPos = findDeclarationEnd(buffer);
			if (endPos !== -1) {
				let sig = buffer.slice(0, endPos).trim();

				// Strip inline keyword
				sig = sig.replace(/\binline\s+/g, "");

				// Prepend className
				if (className) {
					if (!sig.includes(`${className}::${memberName}`)) {
						if (memberName.startsWith("operator")) {
							const opPart = memberName.slice("operator".length).trim();
							const escOp = opPart.replace(/[-[\]{}()*+?.,\\^$|#\s]/g, "\\$&");
							sig = sig.replace(new RegExp(`(?<!::)\\boperator\\s*${escOp}`), `${className}::${memberName}`);
						} else if (memberName.startsWith("~")) {
							const escName = className.replace(/[-[\]{}()*+?.,\\^$|#\s]/g, "\\$&");
							sig = sig.replace(new RegExp(`(?<!::)~\\s*${escName}\\b`), `${className}::~${className}`);
						} else {
							const esc = memberName.replace(/[-[\]{}()*+?.,\\^$|#\s]/g, "\\$&");
							sig = sig.replace(new RegExp(`(?<!::)\\b${esc}\\b`), `${className}::${memberName}`);
						}
					}
				}

				// Format = delete / = default
				sig = sig.replace(/\s*=\s*(delete|default|0)\s*;?$/, " = $1;");

				if (!sig.endsWith(";")) sig += ";";
				return sig;
			}
		}
	}
	return null;
}

function formatFunctionSignature(f, className = "") {
	const fName = cleanSignature(getAllText(findChild(f, "name")));
	const locNode = findChild(f, "location");
	const filePath = locNode?.attributes?.file;
	const lineNum = locNode?.attributes?.line ? parseInt(locNode.attributes.line, 10) : 0;

	if (className && filePath && lineNum > 0) {
		const fromSrc = extractDeclarationFromSource(filePath, lineNum, className, fName);
		if (fromSrc) return fromSrc;
	}

	// Fallback to Doxygen attributes
	const fType = cleanCppType(getAllText(findChild(f, "type")));
	const fArgs = cleanCppArgs(getAllText(findChild(f, "argsstring")));
	const qualifiedName = className ? `${className}::${fName}` : fName;
	return `${fType ? fType + " " : ""}${qualifiedName}${fArgs || "()"};`;
}

function splitQualifiedName(rawName) {
	const parts = [];
	let current = "";
	let depth = 0;
	for (let i = 0; i < rawName.length; i++) {
		const ch = rawName[i];
		if (ch === "<" || ch === "(") {
			depth++;
			current += ch;
		} else if (ch === ">" || ch === ")") {
			depth = Math.max(0, depth - 1);
			current += ch;
		} else if (ch === ":" && depth === 0 && rawName[i + 1] === ":") {
			parts.push(current);
			current = "";
			i++;
		} else current += ch;
	}
	if (current) parts.push(current);
	return parts;
}

const anchorLinkIconSvg = `<span aria-hidden="true" class="sl-anchor-icon"><svg width="16" height="16" viewBox="0 0 24 24" fill="currentColor"><path d="m12.11 15.39-3.88 3.88a2.52 2.52 0 0 1-3.5 0 2.47 2.47 0 0 1 0-3.5l3.88-3.88a1 1 0 1 0-1.42-1.42l-3.88 3.89a4.48 4.48 0 0 0 6.33 6.33l3.89-3.88a1 1 0 0 0-1.42-1.42m8.58-12.08a4.49 4.49 0 0 0-6.33 0l-3.89 3.88a1 1 0 1 0 1.42 1.42l3.88-3.88a2.52 2.52 0 0 1 3.5 0 2.47 2.47 0 0 1 0 3.5l-3.88 3.88a1 1 0 0 0 0 1.42 1 1 0 0 0 1.42 0l3.88-3.89a4.49 4.49 0 0 0 0-6.33M8.83 15.17a1 1 0 0 0 .71.29 1 1 0 0 0 .71-.29l4.92-4.92a1 1 0 1 0-1.42-1.42l-4.92 4.92a1 1 0 0 0 0 1.42"></path></svg></span>`;

function renderNamespaceContent(publicEnums, publicTypes, publicFuncs, publicVars, labels, lang, refMap = null) {
	let md = "";

	if (publicEnums.length > 0) {
		md += `## ${labels.publicEnums}\n\n`;
		for (const e of publicEnums) {
			const eName = cleanSignature(getAllText(findChild(e, "name")));
			const isStrong = e.attributes.strong === "yes";
			const values = findChildren(e, "enumvalue").map((v) => {
				const vName = cleanSignature(getAllText(findChild(v, "name")));
				const vInit = cleanSignature(getAllText(findChild(v, "initializer")));
				return `    ${vName}${vInit ? " " + vInit : ""}`;
			});
			const eDef = values.length > 0
				? `enum ${isStrong ? "class " : ""}${eName} {\n${values.join(",\n")}\n};`
				: `enum ${isStrong ? "class " : ""}${eName} {};`;
			const eBrief = renderDocNode(findChild(e, "briefdescription"), lang, refMap).trim();
			const eDetailed = renderDocNode(findChild(e, "detaileddescription"), lang, refMap).trim();
			md += `### \`${eName}\`\n\n\`\`\`cpp\n${eDef}\n\`\`\`\n\n`;
			if (eBrief) md += `${eBrief}\n\n`;
			if (eDetailed) md += `${eDetailed}\n\n`;
		}
	}

	if (publicTypes.length > 0) {
		md += `## ${labels.publicTypes}\n\n`;
		for (const t of publicTypes) {
			const tName = cleanSignature(getAllText(findChild(t, "name")));
			const tDef = cleanSignature(getAllText(findChild(t, "definition")));
			const tBrief = renderDocNode(findChild(t, "briefdescription"), lang, refMap).trim();
			md += `### \`${tName}\`\n\n\`\`\`cpp\n${tDef || tName};\n\`\`\`\n\n`;
			if (tBrief) md += `${tBrief}\n\n`;
		}
	}

	if (publicFuncs.length > 0) {
		md += `## ${labels.publicFuncs}\n\n`;
		const seenFuncIds = new Set();
		for (const f of publicFuncs) {
			const fName = cleanSignature(getAllText(findChild(f, "name")));
			const fTemplate = extractTemplateParams(f);
			const fBrief = renderDocNode(findChild(f, "briefdescription"), lang, refMap).trim();
			const fDetailed = renderDocNode(findChild(f, "detaileddescription"), lang, refMap).trim();
			const fSig = formatFunctionSignature(f, "");

			let cleanId = sanitizeHtmlId(fName) || "func";
			if (seenFuncIds.has(cleanId)) {
				let c = 2;
				while (seenFuncIds.has(`${cleanId}_${c}`)) c++;
				cleanId = `${cleanId}_${c}`;
			}
			seenFuncIds.add(cleanId);

			const anchorLabel = lang === "fr" ? `Lien direct vers ${cleanId}` : `Direct link to ${cleanId}`;
			md += `<div class="member-signature" id="${cleanId}">\n<a class="member-anchor" href="#${cleanId}" aria-label="${anchorLabel}">${anchorLinkIconSvg}</a>\n\n\`\`\`cpp\n`;
			if (fTemplate && !fSig.startsWith("template")) md += `${fTemplate}\n`;
			md += `${fSig}\n\`\`\`\n\n</div>\n\n`;

			if (fBrief) md += `${fBrief}\n\n`;
			if (fDetailed) md += `${fDetailed}\n\n`;
			md += `---\n\n`;
		}
	}

	if (publicVars.length > 0) {
		md += `## ${labels.publicConstants}\n\n`;
		for (const v of publicVars) {
			const vName = cleanSignature(getAllText(findChild(v, "name")));
			const vType = cleanSignature(getAllText(findChild(v, "type")));
			const vInit = cleanSignature(getAllText(findChild(v, "initializer")));
			const vDef = cleanSignature(getAllText(findChild(v, "definition")));
			const vBrief = renderDocNode(findChild(v, "briefdescription"), lang, refMap).trim();
			md += `### \`${vName}\`\n\n\`\`\`cpp\n${vDef || `${vType} ${vName}${vInit ? " " + vInit : ""}`};\n\`\`\`\n\n`;
			if (vBrief) md += `${vBrief}\n\n`;
		}
	}

	return md;
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
		publicFuncs: isFr ? "Méthodes et fonctions publiques" : "Public methods & functions",
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
	};

	const refMap = new Map();
	const slugCounts = new Map();
	const primaryTemplates = new Map();

	for (const comp of compounds) {
		const rawName = getAllText(findChild(comp, "name")).trim();
		if (!rawName.startsWith("CppUtils::")) continue;
		if (rawName.includes("::detail::") || rawName.includes("::Detail::")) continue;

		const refId = comp.attributes?.refid;
		if (!refId) continue;

		const tokens = splitQualifiedName(rawName);
		const rawSymbolName = tokens.pop();
		const symbolName = cleanTemplateSpaces(rawSymbolName);
		tokens.shift(); // Remove "CppUtils"
		const subNamespace = tokens.join("/");
		const namespaceKey = tokens.join("::") || "Core";

		let safeBaseName = sanitizeFileName(symbolName);
		const slugKey = `${subNamespace}/${safeBaseName}`.toLowerCase();
		let currentCount = slugCounts.get(slugKey) || 0;
		currentCount++;
		slugCounts.set(slugKey, currentCount);
		if (currentCount > 1) safeBaseName = `${safeBaseName}_${currentCount}`;

		const dirPrefix = subNamespace ? `${subNamespace.toLowerCase()}/` : "";
		const url = `/${lang}/reference/${dirPrefix}${safeBaseName.toLowerCase()}/`;
		refMap.set(refId, { url, name: symbolName });

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
						const declname = cleanSignature((p.match(/<declname>([\s\S]*?)<\/declname>/)?.[1] || "").replace(/<[^>]+>/g, ""));
						const defval = cleanSignature((p.match(/<defval>([\s\S]*?)<\/defval>/)?.[1] || "").replace(/<[^>]+>/g, ""));
						let str = [type, declname].filter(Boolean).join(" ");
						if (defval) str += ` = ${defval}`;
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

	const namespacesMap = new Map();
	const usedSlugs = new Set();
	let generatedCount = 0;

	for (const comp of compounds) {
		const kind = comp.attributes.kind;
		const rawName = getAllText(findChild(comp, "name")).trim();
		if (!rawName.startsWith("CppUtils::")) continue;
		if (rawName.includes("::detail::") || rawName.includes("::Detail::")) continue;

		const refId = comp.attributes.refid;
		const xmlPath = path.join(xmlDir, `${refId}.xml`);
		if (!fs.existsSync(xmlPath)) continue;

		const fileContent = fs.readFileSync(xmlPath, "utf-8");
		const fileTree = await parseXml(fileContent);
		const compounddef = findChild(fileTree, "compounddef");
		if (!compounddef) continue;

		const tokens = splitQualifiedName(rawName);
		const rawSymbolName = tokens.pop();
		const symbolName = cleanTemplateSpaces(rawSymbolName);
		tokens.shift(); // Remove "CppUtils"
		const subNamespace = tokens.join("/");
		const namespaceKey = tokens.join("::") || "Core";

		const targetDir = subNamespace ? path.join(outDir, subNamespace) : outDir;
		fs.mkdirSync(targetDir, { recursive: true });

		let safeBaseName = sanitizeFileName(symbolName);
		const slugKey = `${subNamespace}/${safeBaseName}`.toLowerCase();
		if (usedSlugs.has(slugKey)) {
			let counter = 2;
			while (usedSlugs.has(`${slugKey}_${counter}`)) counter++;
			safeBaseName = `${safeBaseName}_${counter}`;
			usedSlugs.add(`${slugKey}_${counter}`);
		} else usedSlugs.add(slugKey);

		const fileName = `${safeBaseName}.mdx`;
		const filePath = path.join(targetDir, fileName);

		const briefDesc = renderDocNode(findChild(compounddef, "briefdescription"), lang, refMap).trim();
		const detailedDesc = renderDocNode(findChild(compounddef, "detaileddescription"), lang, refMap).trim();
		const templateHeader = extractTemplateParams(compounddef);

		const pageDesc = isFr
			? `Référence d'API pour CppUtils::${tokens.concat([symbolName]).join("::")}`
			: `API Reference for CppUtils::${tokens.concat([symbolName]).join("::")}`;

		const nsPrefix = `CppUtils::${tokens.length > 0 ? tokens.join("::") + "::" : ""}`;

		const compLoc = findChild(compounddef, "location");
		const compFile = compLoc?.attributes?.file;
		const moduleName = extractModuleFromSource(compFile) || (tokens.length > 0 ? `CppUtils.${tokens.join(".")}` : "CppUtils");

		const templateBrackets = templateHeader ? templateHeader.replace(/^\s*template\s*/, "").trim() : "";

		const specMatch = symbolName.match(/^([^<]+)(<.+>)$/);
		const baseSymbolName = specMatch ? specMatch[1].trim() : symbolName;
		const specializationArgs = specMatch ? specMatch[2].trim() : "";

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

		if (kind === "concept") {
			const initNode = findChild(compounddef, "initializer");
			const initText = cleanSignature(getAllText(initNode));
			markdown += `## ${labels.declaration}\n\n\`\`\`cpp\n`;
			if (templateHeader) markdown += `${templateHeader}\n`;
			markdown += `${initText || `concept ${symbolName} = ...;`}\n\`\`\`\n\n`;
		}

		if (detailedDesc) markdown += `## ${labels.description}\n\n${detailedDesc}\n\n`;

		if (kind !== "concept") {
			const sections = findChildren(compounddef, "sectiondef");
			const publicFuncs = [];
			const publicTypes = [];
			const publicAttrs = [];

			for (const sec of sections) {
				const secKind = sec.attributes.kind;
				const members = findChildren(sec, "memberdef");
				for (const m of members) {
					const prot = m.attributes.prot;
					if (prot !== "public") continue;

					if (secKind.includes("func")) publicFuncs.push(m);
					else if (secKind.includes("type")) publicTypes.push(m);
					else if (secKind.includes("attrib")) publicAttrs.push(m);
				}
			}

			if (publicTypes.length > 0) {
				markdown += `## ${labels.publicTypes}\n\n`;
				for (const t of publicTypes) {
					const tName = cleanSignature(getAllText(findChild(t, "name")));
					const tDef = cleanSignature(getAllText(findChild(t, "definition")));
					const tBrief = renderDocNode(findChild(t, "briefdescription"), lang, refMap).trim();
					markdown += `### \`${tName}\`\n\n\`\`\`cpp\n${tDef || tName};\n\`\`\`\n\n`;
					if (tBrief) markdown += `${tBrief}\n\n`;
				}
			}

			if (publicFuncs.length > 0) {
				markdown += `## ${labels.publicFuncs}\n\n`;
				const seenFuncIds = new Set();
				for (const f of publicFuncs) {
					const fName = cleanSignature(getAllText(findChild(f, "name")));
					const fTemplate = extractTemplateParams(f);
					const fBrief = renderDocNode(findChild(f, "briefdescription"), lang, refMap).trim();
					const fDetailed = renderDocNode(findChild(f, "detaileddescription"), lang, refMap).trim();
					const fSig = formatFunctionSignature(f, baseSymbolName);

					let cleanId = sanitizeHtmlId(fName) || "method";
					if (seenFuncIds.has(cleanId)) {
						let c = 2;
						while (seenFuncIds.has(`${cleanId}_${c}`)) c++;
						cleanId = `${cleanId}_${c}`;
					}
					seenFuncIds.add(cleanId);

					const anchorLabel = lang === "fr" ? `Lien direct vers ${cleanId}` : `Direct link to ${cleanId}`;
					markdown += `<div class="member-signature" id="${cleanId}">\n<a class="member-anchor" href="#${cleanId}" aria-label="${anchorLabel}">${anchorLinkIconSvg}</a>\n\n\`\`\`cpp\n`;
					if (fTemplate && !fSig.startsWith("template")) markdown += `${fTemplate}\n`;
					markdown += `${fSig}\n\`\`\`\n\n</div>\n\n`;

					if (fBrief) markdown += `${fBrief}\n\n`;
					if (fDetailed) markdown += `${fDetailed}\n\n`;
					markdown += `---\n\n`;
				}
			}

			if (publicAttrs.length > 0) {
				markdown += `## ${labels.publicAttrs}\n\n`;
				for (const a of publicAttrs) {
					const aName = cleanSignature(getAllText(findChild(a, "name")));
					const aType = cleanSignature(getAllText(findChild(a, "type")));
					const aBrief = renderDocNode(findChild(a, "briefdescription"), lang, refMap).trim();
					markdown += `### \`${aName}\`\n\n\`\`\`cpp\n${aType} ${aName};\n\`\`\`\n\n`;
					if (aBrief) markdown += `${aBrief}\n\n`;
				}
			}
		}

		fs.writeFileSync(filePath, markdown, "utf-8");
		generatedCount++;

		if (!namespacesMap.has(namespaceKey)) namespacesMap.set(namespaceKey, []);
		namespacesMap.get(namespaceKey).push({ name: symbolName, kind, file: fileName, subNamespace });
	}

	// Process namespace-level compounds (free functions, enums, constants, typedefs)
	for (const nsComp of namespaceCompounds || []) {
		const rawName = getAllText(findChild(nsComp, "name")).trim();
		if (!rawName.startsWith("CppUtils")) continue;
		if (rawName.includes("::detail") || rawName.includes("::Detail")) continue;

		const refId = nsComp.attributes.refid;
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

		for (const sec of sections) {
			const secKind = sec.attributes.kind;
			const members = findChildren(sec, "memberdef");
			for (const m of members) {
				const prot = m.attributes.prot || "public";
				if (prot !== "public") continue;
				const mId = m.attributes.id;
				if (seenIds.has(mId)) continue;
				seenIds.add(mId);

				if (secKind.includes("func")) publicFuncs.push(m);
				else if (secKind === "enum") publicEnums.push(m);
				else if (secKind === "typedef") publicTypes.push(m);
				else if (secKind === "var") publicVars.push(m);
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

		if (publicFuncs.length > 0) {
			fileName = "Functions.mdx";
			title = labels.namespaceFunctions;
			badgeText = "FUNCTIONS";
			itemKind = "functions";
		} else if (publicEnums.length > 0) {
			fileName = "Enums.mdx";
			title = labels.enums;
			badgeText = "ENUM";
			itemKind = "enum";
		} else if (publicVars.length > 0) {
			fileName = "Constants.mdx";
			title = labels.constants;
			badgeText = "CONSTANTS";
			itemKind = "constants";
		} else {
			fileName = "Types.mdx";
			title = labels.types;
			badgeText = "TYPES";
			itemKind = "types";
		}

		if (!subNamespace) {
			fileName = "GlobalFunctions.mdx";
			title = isFr ? "Fonctions globales CppUtils" : "Global CppUtils Functions";
		}

		const targetDir = subNamespace ? path.join(outDir, subNamespace) : outDir;
		fs.mkdirSync(targetDir, { recursive: true });
		const filePath = path.join(targetDir, fileName);

		const fullNsName = rawName;
		const pageDesc = isFr
			? `${title} du namespace ${fullNsName}`
			: `${title} for namespace ${fullNsName}`;

		const nsModule = fullNsName.replace(/::/g, ".");
		let markdown = `---\ntitle: "${title}"\ndescription: "${pageDesc}"\nbadge: "${badgeText}"\nmoduleImport: "${nsModule}"\n---\n\n`;
		markdown += `**${labels.namespace}** \`${fullNsName}\`\n\n`;

		markdown += renderNamespaceContent(publicEnums, publicTypes, publicFuncs, publicVars, labels, lang, refMap);

		fs.writeFileSync(filePath, markdown, "utf-8");
		generatedCount++;

		if (!namespacesMap.has(namespaceKey)) namespacesMap.set(namespaceKey, []);
		namespacesMap.get(namespaceKey).push({ name: title, kind: itemKind, file: fileName, subNamespace });
	}

function getBadgeInfo(kind) {
	switch (kind?.toLowerCase()) {
		case "class":
			return { badgeText: "CLASS", badgeClass: "ref-badge ref-badge-class" };
		case "struct":
			return { badgeText: "STRUCT", badgeClass: "ref-badge ref-badge-struct" };
		case "concept":
			return { badgeText: "CONCEPT", badgeClass: "ref-badge ref-badge-concept" };
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

	// Create Reference Overview Index page
	let indexMd = `---\ntitle: "${labels.apiRefTitle}"\ndescription: "${labels.apiRefDesc}"\n---\n\n`;
	indexMd += `export const base = import.meta.env.BASE_URL.replace(/\\/$/, '');\n\n`;

	const sortedNamespaces = [...namespacesMap.keys()].sort();

	indexMd += `<div class="reference-container">\n`;
	indexMd += `  <div class="reference-masonry">\n`;
	for (const ns of sortedNamespaces) {
		const items = namespacesMap.get(ns).sort((a, b) => a.name.localeCompare(b.name));

		indexMd += `    <div class="reference-category-card">\n`;
		indexMd += `      <div class="reference-card-header">\n`;
		indexMd += `        <h3 class="reference-card-title"><code>CppUtils::${ns}</code></h3>\n`;
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
}

main().catch((err) => {
	console.error("[generate-api] Fatal error:", err);
	process.exit(1);
});
