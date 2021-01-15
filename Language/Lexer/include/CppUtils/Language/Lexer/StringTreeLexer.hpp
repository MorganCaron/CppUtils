#pragma once

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>

namespace CppUtils::Language::Lexer
{
	class StringTreeLexer final
	{
	public:
		using TreeNode = Graph::VariantTreeNode<Type::Token>;

		StringTreeLexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;

			m_grammarLexer.addParsingFunction("spaceParser"_token, Parser::spaceParser<Type::Token>);
			m_grammarLexer.addParsingFunction("quoteParser"_token, Parser::quoteParser<Type::Token>);

			static constexpr auto grammarSrc = R"(
			main: (_node >= 0) spaceParser;
			_node: spaceParser [quoteParser] ~_childs;
			_childs: spaceParser '{' (_node >= 1) spaceParser '}';
			)"sv;
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] inline TreeNode parse(const std::string_view src) const
		{
			using namespace Type::Literals;
			return m_grammarLexer.parseLanguage("main"_token, src);
		}

	private:
		GrammarLexer<Type::Token> m_grammarLexer;
	};

	namespace StringTree
	{
		[[nodiscard]] inline StringTreeLexer::TreeNode parse(const std::string_view src)
		{
			static const auto stringTreeLexer = CppUtils::Language::Lexer::StringTreeLexer{};
			return stringTreeLexer.parse(src);
		}
	}
}
