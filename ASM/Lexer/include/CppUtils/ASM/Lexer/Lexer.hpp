#pragma once

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>

namespace CppUtils::ASM
{
	template<typename... Types>
	class Lexer final
	{
	public:
		Lexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;

			m_grammarLexer.addParsingFunction("spaceParser"_token, Language::Parser::spaceParser<Type::Token, Types...>);
			m_grammarLexer.addParsingFunction("keywordParser"_token, Language::Parser::keywordParser<Type::Token, Types...>);
			m_grammarLexer.addParsingFunction("intParser"_token, Language::Parser::intParser<Type::Token, Types...>);

			static constexpr auto grammarSrc = R"(
			main: (_instruction >= 0) spaceParser;
			_instruction: spaceParser (halt || nop || move || add);
			_token: spaceParser keywordParser;
			_variable: _token;
			variable: _variable;
			_value: spaceParser int;
			int: intParser;
			value: _value;
			halt: "hlt";
			nop: "nop";
			move: "mov" _variable spaceParser ',' (value || variable);
			add: "add" _variable spaceParser ',' (value || variable);
			)"sv;
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] inline Language::Parser::ASTNode<Type::Token, Types...> parse(const std::string_view src) const
		{
			using namespace Type::Literals;
			return m_grammarLexer.parseLanguage("main"_token, src);
		}

	private:
		Language::Lexer::GrammarLexer<Type::Token, Types...> m_grammarLexer;
	};

	template<typename... Types>
	[[nodiscard]] inline Language::Parser::ASTNode<Type::Token, Types...> parse(const std::string_view src)
	{
		static const auto stackIRLexer = Lexer<Types...>{};
		return stackIRLexer.parse(src);
	}
}
