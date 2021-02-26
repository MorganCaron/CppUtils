#pragma once

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>

namespace CppUtils::Language::StringTree
{
	class StringTreeLexer final
	{
	public:
		using ASTNode = Parser::ASTNode<Type::Token>;

		StringTreeLexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;

			m_grammarLexer.addParsingFunction("spaceParser"_token, Parser::spaceParser<Type::Token>);
			m_grammarLexer.addParsingFunction("quoteParser"_token, Parser::quoteParser<Type::Token>);

			static constexpr auto grammarSrc = R"(
			main: (_node >= 0) spaceParser;
			_string: quoteParser;
			_node: spaceParser [_string] ~_childs;
			_childs: spaceParser '{' (_node >= 1) spaceParser '}';
			)"sv;
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] inline ASTNode parse(const std::string_view src) const
		{
			using namespace Type::Literals;
			return m_grammarLexer.parseLanguage("main"_token, src);
		}

	private:
		Lexer::GrammarLexer<Type::Token> m_grammarLexer;
	};

	[[nodiscard]] inline StringTreeLexer::ASTNode parse(const std::string_view src)
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
