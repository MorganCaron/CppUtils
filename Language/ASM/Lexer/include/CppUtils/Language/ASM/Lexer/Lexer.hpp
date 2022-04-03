#pragma once

#include <CppUtils/Type/Traits.hpp>
#include <CppUtils/Language/Lexer/GrammarLexer.hpp>

namespace CppUtils::Language::ASM::Lexer
{
	using ASTNode = Parser::ASTNode<Type::Token, std::uintptr_t>;

	class Lexer final
	{
	public:
		Lexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;

			m_grammarLexer.addParsingFunction("spaceParser"_token, Parser::spaceParser<Type::Token, std::uintptr_t>);
			m_grammarLexer.addParsingFunction("keywordParser"_token, Parser::keywordParser<Type::Token, std::uintptr_t>);
			m_grammarLexer.addParsingFunction("addressParser"_token, Parser::ulongParser<Type::Token, std::uintptr_t>);

			static constexpr auto grammarSrc = R"(
			main: (instruction >= 0) spaceParser;
			!instruction: ~[label] spaceParser (halt || nop || move || add);

			!token: spaceParser keywordParser;
			label: token spaceParser ':';
			!_number: addressParser;
			number: _number;
			!_value: spaceParser number;
			value: _value;

			!_register: spaceParser 'r' _number;
			register: _register;

			halt: "hlt";
			nop: "nop";
			move: "mov" _register spaceParser ',' (_value || register);
			add: "add" _register spaceParser ',' (_value || register);
			)"sv;
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] ASTNode parse(std::string_view src) const
		{
			using namespace Type::Literals;
			return m_grammarLexer.parseLanguage("main"_token, src);
		}

	private:
		Language::Lexer::GrammarLexer<Type::Token, std::uintptr_t> m_grammarLexer;
	};

	[[nodiscard]] ASTNode parse(std::string_view src)
	{
		static const auto asmLexer = Lexer{};
		return asmLexer.parse(src);
	}
}
