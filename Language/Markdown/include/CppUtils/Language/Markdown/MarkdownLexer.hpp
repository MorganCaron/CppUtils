#pragma once

#include <CppUtils/Language/Lexer/Grammar.hpp>

namespace CppUtils::Language::Markdown
{
	using namespace std::literals;

	constexpr auto markdownGrammarSrc = R"(
		main: _content spaceParser;
		content: _content;
		!_content: (_element >= 0);
		!_element: spaceParser (_header || checkbox || image || link || boldAndItalic || bold || italic || blockcode || code || _char);
		!_header: spaceParser '#' (h6 || h5 || h4 || h3 || h2 || h1);
		h1: spaceParser headerContent;
		h2: "#" spaceParser headerContent;
		h3: "##" spaceParser headerContent;
		h4: "###" spaceParser headerContent;
		h5: "####" spaceParser headerContent;
		h6: "#####" spaceParser headerContent;
		headerContent[content]: ((_char != '\n') >= 0);
		checkbox: (_emptyCheckbox || checkedCheckbox);
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
		bold: (_bold1 || _bold2);
		!_bold1: "**" ((_element != "**") >= 0) "**";
		!_bold2: "__" ((_element != "__") >= 0) "__";
		italic: (_italic1 || _italic2);
		!_italic1: '*' ((_element != '*') >= 0) '*';
		!_italic2: '_' ((_element != '_') >= 0) '_';
		blockcode: "```" ~blockcodeAttributes blockcodeContent "```";
		blockcodeAttributes[attributes]: blockcodeLang;
		blockcodeLang[lang]: keywordParser;
		blockcodeContent[content]: spaceParser ((_char != "```") >= 0) trimModifier;
		code: '`' codeContent '`';
		codeContent[content]: ((_char != '`') >= 0);
		!_char: contentParser;
	)"sv;
	
	[[nodiscard]] auto parse(std::string_view src) -> Parser::Ast
	{
		const auto lowLevelGrammarAst = Parser::parseAst(Lexer::Grammar::lowLevelGrammarSrc);
		const auto highLevelGrammarAst = Lexer::parse(Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
		const auto markdownGrammarAst = Lexer::parse(markdownGrammarSrc, highLevelGrammarAst);
		return Lexer::parse(src, markdownGrammarAst);
	}

	namespace Literals
	{
		[[nodiscard]] auto operator"" _markdown(const char* cString, std::size_t) -> Parser::Ast
		{
			return parse(cString);
		}
	}
}
