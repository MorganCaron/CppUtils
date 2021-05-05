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

			static const auto formatParser = [this](auto& context) -> bool {
				return m_grammarLexer.parseSegment("_element"_token, context);
			};

			m_grammarLexer.addParsingFunction("spaceParser"_token, Parser::spaceParser<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("keywordParser"_token, Parser::keywordParser<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("contentParser"_token, Parser::escapeCharParser<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("headerContentParser"_token, std::bind(Parser::delimiterParser<Type::Token, std::string>, _1, "\n"sv, Parser::escapeCharParser<Type::Token, std::string>, true, false));
			m_grammarLexer.addParsingFunction("altContentParser"_token, std::bind(Parser::delimiterParser<Type::Token, std::string>, _1, "]"sv, Parser::escapeCharParser<Type::Token, std::string>, true, false));
			m_grammarLexer.addParsingFunction("srcContentParser"_token, std::bind(Parser::delimiterParser<Type::Token, std::string>, _1, ")"sv, Parser::escapeCharParser<Type::Token, std::string>, true, false));
			m_grammarLexer.addParsingFunction("blockcodeContentParser"_token, std::bind(Parser::delimiterParser<Type::Token, std::string>, _1, "```"sv, Parser::escapeCharParser<Type::Token, std::string>, true, false));
			m_grammarLexer.addParsingFunction("codeContentParser"_token, std::bind(Parser::delimiterParser<Type::Token, std::string>, _1, "`"sv, Parser::escapeCharParser<Type::Token, std::string>, true, false));
			m_grammarLexer.addParsingFunction("trimModifier"_token, Parser::Modifier::trimModifier<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("bold1ContentParser"_token, std::bind(Parser::delimiterParser<Type::Token, std::string>, _1, "**"sv, formatParser, true, false));
			m_grammarLexer.addParsingFunction("bold2ContentParser"_token, std::bind(Parser::delimiterParser<Type::Token, std::string>, _1, "__"sv, formatParser, true, false));
			m_grammarLexer.addParsingFunction("italic1ContentParser"_token, std::bind(Parser::delimiterParser<Type::Token, std::string>, _1, "*"sv, formatParser, true, false));
			m_grammarLexer.addParsingFunction("italic2ContentParser"_token, std::bind(Parser::delimiterParser<Type::Token, std::string>, _1, "_"sv, formatParser, true, false));
			m_grammarLexer.addParsingFunction("boldAndItalicContentParser"_token, std::bind(Parser::delimiterParser<Type::Token, std::string>, _1, "___"sv, formatParser, true, false));

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
			headerContent[content]: headerContentParser;
			checkbox: (_emptyCheckbox || checkedCheckbox);
			!_emptyCheckbox: "[ ]";
			checkedCheckbox[attributes]: checked;
			checked: "[x]";
			image: imageAttributes;
			imageAttributes[attributes]: "![" alt "](" src ')';
			link: linkAttributes;
			linkAttributes[attributes]: '[' alt "](" src ')';
			alt: altContentParser;
			src: srcContentParser;
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
			blockcodeContent[content]: spaceParser blockcodeContentParser trimModifier;
			code: '`' codeContent '`';
			codeContent[content]: codeContentParser;
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
