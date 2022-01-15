#pragma once

#include <CppUtils/Type/Traits.hpp>
#include <CppUtils/Language/Lexer/GrammarLexer.hpp>

namespace CppUtils::Language::ASM::Lexer
{
	template<typename Address>
	requires Type::Traits::isAddress<Address>
	using ASTNode = Parser::ASTNode<Type::Token, Address>;

	template<typename Address>
	requires Type::Traits::isAddress<Address>
	class Lexer final
	{
	public:
		Lexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;

			m_grammarLexer.addParsingFunction("spaceParser"_token, Parser::spaceParser<Type::Token, Address>);
			m_grammarLexer.addParsingFunction("keywordParser"_token, Parser::keywordParser<Type::Token, Address>);
			auto addressParser = []() -> Parser::ParsingFunction<Type::Token, Address> {
				if constexpr(sizeof(Address) >= 8) {
					if constexpr(std::is_signed_v<Address>)	return Parser::longParser<Type::Token, Address>;
					else									return Parser::ulongParser<Type::Token, Address>;
				} else {
					if constexpr(std::is_signed_v<Address>)	return Parser::intParser<Type::Token, Address>;
					else									return Parser::uintParser<Type::Token, Address>;
				}
			}();
			m_grammarLexer.addParsingFunction("addressParser"_token, std::move(addressParser));

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

		[[nodiscard]] ASTNode<Address> parse(std::string_view src) const
		{
			using namespace Type::Literals;
			return m_grammarLexer.parseLanguage("main"_token, src);
		}

	private:
		Language::Lexer::GrammarLexer<Type::Token, Address> m_grammarLexer;
	};

	template<typename Address>
	requires Type::Traits::isAddress<Address>
	[[nodiscard]] inline ASTNode<Address> parse(std::string_view src)
	{
		static const auto asmLexer = Lexer<Address>{};
		return asmLexer.parse(src);
	}
}
