#pragma once

#include <CppUtils/Language/Lexer/Grammar.hpp>

namespace CppUtils::Language::Markdown
{
	using namespace std::literals;
	using namespace CppUtils::Language::Lexer::Grammar::Literals;

	[[nodiscard]] inline auto getMarkdownGrammarAst() -> Parser::Ast
	{
		static const auto markdownGrammarAst = R"(
			main: _content spaceParser;
			content: _content;
			!_content: (_element >= 0);
			!_element: spaceParser (_header or checkbox or image or link or boldAndItalic or bold or italic or blockcode or code or _char);
			!_header: spaceParser '#' (h6 or h5 or h4 or h3 or h2 or h1);
			h1: spaceParser headerContent;
			h2: "#" spaceParser headerContent;
			h3: "##" spaceParser headerContent;
			h4: "###" spaceParser headerContent;
			h5: "####" spaceParser headerContent;
			h6: "#####" spaceParser headerContent;
			headerContent[content]: ((_char != '\n') >= 0);
			checkbox: (_emptyCheckbox or checkedCheckbox);
			!_emptyCheckbox: "[ ]";
			checkedCheckbox[attributes]: checked;
			checked: "[x]";
			image: imageAttributes;
			imageAttributes[attributes]: "![" alt "](" src ')';
			link: linkAttributes;
			linkAttributes[attributes]: '[' alt "](" src ')';
			alt: ((_char != ']') >= 0);
			src: ((_char != ')') >= 0);
			boldAndItalic[bold]: "___" boldAndItalic2 "___";
			boldAndItalic2[italic]: ((_element != "___") >= 0);
			bold: (_bold1 or _bold2);
			!_bold1: "**" ((_element != "**") >= 0) "**";
			!_bold2: "__" ((_element != "__") >= 0) "__";
			italic: (_italic1 or _italic2);
			!_italic1: '*' ((_element != '*') >= 0) '*';
			!_italic2: '_' ((_element != '_') >= 0) '_';
			blockcode: "```" ~blockcodeAttributes blockcodeContent "```";
			blockcodeAttributes[attributes]: blockcodeLang;
			blockcodeLang[lang]: keywordParser;
			blockcodeContent[content]: spaceParser ((_char != "```") >= 0) trimModifier;
			code: '`' codeContent '`';
			codeContent[content]: ((_char != '`') >= 0);
			!_char: contentParser;
		)"_grammar;
		return markdownGrammarAst;
	}

	[[nodiscard]] auto parse(std::string_view src) -> Parser::Ast
	{
		return Lexer::parse(src, getMarkdownGrammarAst());
	}

	namespace Literals
	{
		[[nodiscard]] auto operator""_markdown(const char* cString, std::size_t) -> Parser::Ast
		{
			return parse(cString);
		}
	}
}
