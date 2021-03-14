#pragma once

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>

namespace CppUtils::Language::IR::Lexer
{
	template<typename Address> requires std::is_integral_v<Address>
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
			main: (label >= 0) spaceParser;

			_functionDeclaration: [_functionName] _argumentsDeclaration _instructions;
			label: _functionDeclaration;
			_instructions: spaceParser '{' (_instruction >= 0) spaceParser '}';
			instructions: _instructions;
			_token: spaceParser keywordParser;
			_variable: _token;
			variable: _variable;
			ident: _variable;
			_functionName: _token;
			_comma: spaceParser ',';
			_value: spaceParser (_parenthesis || _literal || ident);
			_parenthesis: '(' _operation spaceParser ')';
			
			_operation: _operand;
			_operand: _value ~[_secondOperand];
			_secondOperand: [_operator] _operand;

			_argumentsDeclaration: spaceParser '(' ~_argumentDeclaration spaceParser ')';
			_argumentDeclaration: _variable ~_secondArgumentDeclaration;
			_secondArgumentDeclaration: _comma _argumentDeclaration;

			_arguments: '(' ~_argument spaceParser ')';
			_argument: _operation ~_secondArgument;
			_secondArgument: _comma _argument;

			_literal: number;
			number: addressParser;

			_operator: spaceParser (add || neg);
			add: '+';
			neg: '-';

			_instruction: spaceParser (halt || nop || copy || call || ret) spaceParser ';';
			halt: "halt";
			nop: "nop";
			copy: ident spaceParser '=' _operation;
			call: "call" _functionName _arguments;
			ret: "ret" _value;
			)"sv;
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] inline Parser::ASTNode<Type::Token, Address> parse(std::string_view src) const
		{
			using namespace Type::Literals;
			return m_grammarLexer.parseLanguage("main"_token, src);
		}

	private:
		Language::Lexer::GrammarLexer<Type::Token, Address> m_grammarLexer;
	};

	template<typename Address> requires std::is_integral_v<Address>
	[[nodiscard]] inline Parser::ASTNode<Type::Token, Address> parse(std::string_view src)
	{
		static const auto irLexer = Lexer<Address>{};
		return irLexer.parse(src);
	}
}
