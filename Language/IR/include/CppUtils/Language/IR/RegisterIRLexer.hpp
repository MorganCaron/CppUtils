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

			m_grammarLexer.addParsingFunction("spaceParser"_token, Parser::spaceParser<Types...>);
			m_grammarLexer.addParsingFunction("keywordParser"_token, Parser::keywordParser<Types...>);

			static constexpr auto grammarSrc = R"(
			main: (instruction >= 0) spaceParser;
			!instruction: spaceParser (lhsFunction || assignment);
			assignment: _variable spaceParser '=' spaceParser (rhsFunction || operation);
			operation: variable operator variable;
			!token: spaceParser keywordParser;
			!_variable: token;
			variable: _variable;

			lhsFunction: store;
			store: "store" variable variable;

			rhsFunction: load;
			load: "load" _variable;

			!operator: spaceParser (add || sub || mul || div);
			add: '+';
			sub: '-';
			mul: '*';
			div: '/';
			)"sv;
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] inline Parser::ASTNode<Types...> parse(std::string_view src) const
		{
			using namespace Type::Literals;
			return m_grammarLexer.parseLanguage("main"_token, src);
		}

	private:
		Lexer::GrammarLexer<Types...> m_grammarLexer;
	};

	namespace RegisterIR
	{
		template<typename... Types>
		[[nodiscard]] inline Parser::ASTNode<Types...> parse(std::string_view src)
		{
			static const auto registerIRLexer = RegisterIRLexer<Types...>{};
			return registerIRLexer.parse(src);
		}
	}
}
