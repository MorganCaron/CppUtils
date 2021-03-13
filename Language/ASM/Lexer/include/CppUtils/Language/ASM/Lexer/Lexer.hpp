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
			_instruction: ~[label] spaceParser (halt || nop || move || add);

			_token: spaceParser keywordParser;
			label: _token spaceParser ':';
			_number: ulongParser;
			number: _number;
			_value: spaceParser number;
			value: _value;

			_register: spaceParser 'r' _number;
			register: _register;

			halt: "hlt";
			nop: "nop";
			move: "mov" _register spaceParser ',' (_value || register);
			add: "add" _register spaceParser ',' (_value || register);
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
