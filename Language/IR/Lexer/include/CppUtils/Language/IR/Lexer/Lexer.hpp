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
			m_grammarLexer.addParsingFunction("quoteParser"_token, Parser::quoteParser<Type::Token, Address>);

			static constexpr auto grammarSrc = R"(
			main[comma]: (label >= 0) spaceParser;

			!functionDeclaration: functionName ~parameters instructions;
			instructions[comma]: spaceParser '{' (instruction >= 0) spaceParser '}';
			!token: spaceParser keywordParser;
			!comma: spaceParser ',';
			!variable: token;
			!functionName: token;
			!functionCall: functionName arguments;
			!parenthesis: spaceParser '(' operand spaceParser ')';

			!argumentsDeclaration: spaceParser '(' ~argumentDeclaration spaceParser ')';
			!argumentDeclaration: variable ~secondArgumentDeclaration;
			!secondArgumentDeclaration: comma argumentDeclaration;

			!arguments: '(' ~argument spaceParser ')';
			!argument: rvalue ~secondArgument;
			!secondArgument: comma argument;
			
			!assignment: lvalue [assignmentOperator] operand;
			!operand: rvalue ~[secondOperand];
			!secondOperand: [operator] operand;

			label: functionDeclaration;
			!parameters: !argumentsDeclaration;
			ident: variable;
			number: addressParser;
			string: quoteParser;

			!literal: spaceParser (number || string);
			ref: spaceParser '&' spaceParser rvalue;
			deref: spaceParser '*' spaceParser rvalue;
			!lvalue: (deref || ident);
			!rvalue: (parenthesis || ref || deref || call || literal || ident);

			!assignmentOperator: spaceParser (assignOperator || addAssignOperator || subAssignOperator);
			assignOperator[copy]: '=';
			addAssignOperator[add]: '+=';
			subAssignOperator[sub]: '-=';

			!operator: spaceParser (equalOperator || notEqualOperator || addOperator || subOperator);
			equalOperator[eq]: '==';
			notEqualOperator[neq]: '!=';
			addOperator[add]: '+';
			subOperator[sub]: '-';

			!controlStructure: (if || while);
			!statement: (halt || nop || ret || call || copy) spaceParser ';';
			!instruction: spaceParser (controlStructure || statement);
			halt: "halt";
			nop: "nop";
			ret: "ret" rvalue;
			if: "if" parenthesis (instructions || instruction);
			while: "while" parenthesis (instructions || instruction);
			call: functionCall;
			!copy: assignment;
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
