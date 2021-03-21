#pragma once

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>

namespace CppUtils::Language::IR
{
	template<typename... Types>
	class StackIRLexer final
	{
	public:
		StackIRLexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;

			m_grammarLexer.addParsingFunction("spaceParser"_token, Parser::spaceParser<Types...>);
			m_grammarLexer.addParsingFunction("keywordParser"_token, Parser::keywordParser<Types...>);

			static constexpr auto grammarSrc = R"(
			main: (instruction >= 0) spaceParser;
			!instruction: spaceParser (load || add || sub || mul || div || store);
			!token: spaceParser keywordParser;
			variable: token;
			load: "load" variable;
			add: "add";
			sub: "sub";
			mul: "mul";
			div: "div";
			store: "store" variable;
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

	namespace StackIR
	{
		template<typename... Types>
		[[nodiscard]] inline Parser::ASTNode<Types...> parse(std::string_view src)
		{
			static const auto stackIRLexer = StackIRLexer<Types...>{};
			return stackIRLexer.parse(src);
		}
	}
}
