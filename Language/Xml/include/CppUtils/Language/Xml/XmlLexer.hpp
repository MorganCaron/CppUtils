#pragma once

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>

namespace CppUtils::Language::Xml
{
	class XmlLexer final
	{
	public:
		XmlLexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;

			static const auto textParser = [](auto& context) -> bool {
				auto& [cursor, parentNode] = context;
				const auto string = std::string{String::trimString(cursor.getStringAndSkipItIf([](char c) -> bool {
					return c != '<';
				}))};
				if (string.empty())
					return false;
				parentNode.get().childs.emplace_back(Parser::ASTNode<Type::Token, std::string>{std::move(string)});
				return true;
			};

			m_grammarLexer.addParsingFunction("spaceParser"_token, Parser::spaceParser<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("keywordParser"_token, Parser::keywordParser<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("quoteParser"_token, Parser::quoteParser<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("textParser"_token, textParser);

			static constexpr auto grammarSrc = R"(
			main: tags spaceParser;
			!tags: (tag >= 0);
			!tag: [openingTag] content closingTag;
			!openingTag: spaceParser '<' [tagName] attributes spaceParser '>';
			!closingTag: spaceParser "</" tagName spaceParser '>';
			!tagName: spaceParser keywordParser;
			attributes: (attribute >= 0);
			!attribute: spaceParser attributeName;
			!attributeName: spaceParser keywordParser;
			!attributeValue: spaceParser quoteParser;
			content: (text || tag) ~content;
			!text: spaceParser textParser;
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
		static const auto xmlLexer = XmlLexer{};
		return xmlLexer.parse(src);
	}

	namespace Literals
	{
		[[nodiscard]] Parser::ASTNode<Type::Token, std::string> operator"" _xml(const char* cstring, std::size_t)
		{
			return parse(cstring);
		}
	}
}
