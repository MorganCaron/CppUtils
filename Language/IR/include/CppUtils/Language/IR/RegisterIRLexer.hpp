#pragma once

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>

namespace CppUtils::Language::IR
{
	template<typename... Types>
	class RegisterIRLexer final
	{
	public:
		RegisterIRLexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;

			m_grammarLexer.addParsingFunction("spaceParser"_token, Parser::spaceParser<Type::Token, Types...>);
			m_grammarLexer.addParsingFunction("keywordParser"_token, Parser::keywordParser<Type::Token, Types...>);

			static constexpr auto grammarSrc = R"(
			main: (_instruction >= 0) spaceParser;
			_instruction: spaceParser (lhsFunction || assignment);
			assignment: _variable spaceParser '=' spaceParser (rhsFunction || operation);
			operation: variable _operator variable;
			_token: spaceParser keywordParser;
			_variable: _token;
			variable: _variable;

			lhsFunction: store;
			store: "store" variable variable;

			rhsFunction: load;
			load: "load" _variable;

			_operator: spaceParser (add || sub || mul || div);
			add: '+';
			sub: '-';
			mul: '*';
			div: '/';
			)"sv;
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] inline Graph::VariantTreeNode<Type::Token, Types...> parse(const std::string_view src) const
		{
			using namespace Type::Literals;
			return m_grammarLexer.parseLanguage("main"_token, src);
		}

	private:
		Lexer::GrammarLexer<Type::Token, Types...> m_grammarLexer;
	};

	namespace RegisterIR
	{
		template<typename... Types>
		[[nodiscard]] inline Graph::VariantTreeNode<Type::Token, Types...> parse(const std::string_view src)
		{
			static const auto registerIRLexer = RegisterIRLexer<Types...>{};
			return registerIRLexer.parse(src);
		}
	}
}
