#pragma once

#include <CppUtils/Type/Concepts.hpp>
#include <CppUtils/Language/Lexer/GrammarLexer.hpp>

namespace CppUtils::Language::ASM::Lexer
{
	template<typename... Types> requires Type::Concept::isPresent<Type::Token, Types...>
	class Lexer final
	{
	public:
		Lexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;

			m_grammarLexer.addParsingFunction("spaceParser"_token, Parser::spaceParser<Types...>);
			m_grammarLexer.addParsingFunction("keywordParser"_token, Parser::keywordParser<Types...>);
			m_grammarLexer.addParsingFunction("ulongParser"_token, Parser::ulongParser<Types...>);

			static constexpr auto grammarSrc = R"(
			main: (_instruction >= 0) spaceParser;
			_instruction: spaceParser (halt || nop || move || add);
			_token: spaceParser keywordParser;
			_variable: _token;
			variable: _variable;
			_value: spaceParser number;
			number: ulongParser;
			halt: "hlt";
			nop: "nop";
			move: "mov" _variable spaceParser ',' (_value || variable);
			add: "add" _variable spaceParser ',' (_value || variable);
			)"sv;
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] inline Parser::ASTNode<Types...> parse(std::string_view src) const
		{
			using namespace Type::Literals;
			return m_grammarLexer.parseLanguage("main"_token, src);
		}

	private:
		Language::Lexer::GrammarLexer<Types...> m_grammarLexer;
	};

	template<typename... Types> requires Type::Concept::isPresent<Type::Token, Types...>
	[[nodiscard]] inline Parser::ASTNode<Types...> parse(std::string_view src)
	{
		static const auto asmLexer = Lexer<Types...>{};
		return asmLexer.parse(src);
	}
}
