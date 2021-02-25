#pragma once

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>

namespace CppUtils::Language::IR::Lexer
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
			main: (_functionDeclaration >= 0) spaceParser;

			_functionDeclaration: [_functionName] _argumentsDeclaration _instructions;
			_instructions: spaceParser '{' (_instruction >= 0) spaceParser '}';
			instructions: _instructions;
			_token: spaceParser keywordParser;
			_variable: _token;
			variable: _variable;
			_functionName: _token;
			_comma: spaceParser ',';
			_value: spaceParser (_parenthesis || _literal || variable);
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

			_literal: int;
			int: intParser;

			_operator: spaceParser (add || sub || mul || div);
			add: '+';
			sub: '-';
			mul: '*';
			div: '/';

			_instruction: spaceParser (halt || nop || copy || call || ret) spaceParser ';';
			halt: "halt";
			nop: "nop";
			copy: _variable spaceParser '=' _operation;
			call: "call" _functionName _arguments;
			ret: "ret" _value;
			)"sv;
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] inline Graph::VariantTreeNode<Type::Token, Types...> parse(const std::string_view src) const
		{
			using namespace Type::Literals;
			return m_grammarLexer.parseLanguage("main"_token, src);
		}

	private:
		Language::Lexer::GrammarLexer<Type::Token, Types...> m_grammarLexer;
	};

	template<typename... Types>
	[[nodiscard]] inline Graph::VariantTreeNode<Type::Token, Types...> parse(const std::string_view src)
	{
		static const auto irLexer = Lexer<Types...>{};
		return irLexer.parse(src);
	}
}
