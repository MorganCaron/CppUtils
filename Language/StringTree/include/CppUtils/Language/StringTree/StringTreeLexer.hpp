#pragma once

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>
#include <CppUtils/Language/Parser/Modifiers.hpp>

namespace CppUtils::Language::StringTree
{
	class StringTreeLexer final
	{
	public:
		using ASTNode = Parser::ASTNode<Type::Token, std::string>;

		StringTreeLexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;
			using namespace std::placeholders;

			m_grammarLexer.addParsingFunction("spaceParser"_token, Parser::spaceParser<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("quoteParser"_token, Parser::quoteParser<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("stringToTokenModifier"_token, std::bind(Parser::Modifier::stringToTokenModifier<Type::Token, std::string>, _1, true));

			static constexpr auto grammarSrc = R"(
			main: (node >= 0) spaceParser;
			!string: quoteParser;
			!token: 't' quoteParser stringToTokenModifier;
			!value: spaceParser (string || token);
			!node: [value] ~childs;
			!childs: spaceParser '{' (node >= 1) spaceParser '}';
			)"sv;
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] inline ASTNode parse(std::string_view src) const
		{
			using namespace Type::Literals;
			return m_grammarLexer.parseLanguage("main"_token, src);
		}

	private:
		Lexer::GrammarLexer<Type::Token, std::string> m_grammarLexer;
	};

	[[nodiscard]] inline StringTreeLexer::ASTNode parse(std::string_view src)
	{
		static const auto stringTreeLexer = StringTreeLexer{};
		return stringTreeLexer.parse(src);
	}

	namespace Literals
	{
		[[nodiscard]] StringTreeLexer::ASTNode operator"" _stringTree(const char* cstring, std::size_t)
		{
			return parse(cstring);
		}
	}
}
