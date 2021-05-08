#pragma once

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>

namespace CppUtils::Language::Markdown
{
	class MarkdownLexer final
	{
	public:
		MarkdownLexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;
			using namespace std::placeholders;

			m_grammarLexer.addParsingFunction("spaceParser"_token, Parser::spaceParser<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("keywordParser"_token, Parser::keywordParser<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("contentParser"_token, Parser::escapeCharParser<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("trimModifier"_token, Parser::Modifier::trimModifier<Type::Token, std::string>);
			
			static constexpr auto grammarSrc = R"(
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
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] inline Parser::ASTNode<Type::Token, std::string> parse(std::string_view src) const
		{
			using namespace Type::Literals;
			return m_grammarLexer.parseLanguage("main"_token, src);
		}

	private:
		Lexer::GrammarLexer<Type::Token, std::string> m_grammarLexer;
	};

	[[nodiscard]] inline Parser::ASTNode<Type::Token, std::string> parse(std::string_view src)
	{
		static const auto markdownLexer = MarkdownLexer{};
		return markdownLexer.parse(src);
	}

	namespace Literals
	{
		[[nodiscard]] Parser::ASTNode<Type::Token, std::string> operator"" _markdown(const char* cstring, std::size_t)
		{
			return parse(cstring);
		}
	}
}
