#pragma once

#include <CppUtils/Language/Lexer/Grammar.hpp>

namespace CppUtils::Language::Json
{
	using namespace std::literals;
	using namespace CppUtils::Language::Lexer::Grammar::Literals;

	[[nodiscard]] inline auto getJsonGrammarAst() -> Parser::Ast
	{
		static const auto jsonGrammarAst = R"(
			main: ~spaces object ~spaces;

			# TYPES
			value: ~spaces or(objectValue, stringValue, number, array, boolean, null);
			
			# OBJECT
			object: '{' ~repeat(pair, (~spaces ',')) ~spaces '}';
			objectValue: '{' add(object) sub(~repeat(pair, (~spaces ','))) ~spaces '}';
			pair:  ~spaces hash(key) ~spaces ':' sub(value);
			key: string;

			# STRING
			string: '"' ~repeat(nonQuote) '"';
			stringValue: '"' add(string) sub(~hash(repeat(nonQuote))) '"';
			nonQuote: !'"' read()+;

			# NUMBER
			number: or(is('-'), ['0', '9']) add(number) sub(hash(~minus or(zero, repeat(digit)) ~decimalPart ~exponentPart));
			sign: or(plus, minus);
			plus: '+' add('+');
			minus: '-' add('-');
			digit: ['0', '9'] read()+;
			zero: '0' add('0');
			decimalPart: '.' add('.') repeat(digit);
			exponentPart: or('e', 'E') add('e') ~sign repeat(digit);
			
			# ARRAY
			array: '[' add(array) sub(values) ~spaces ']';
			values: repeat(value, (~spaces ','));

			# CONSTANTS
			boolean: or(true, false);
			true: "true" add(true);
			false: "false" add(false);
			null: "null" add(null);

			# SPACES
			spaces: repeat(space);
			space: or(' ', '\n', '\t', '\r');
		)"_grammar;
		return jsonGrammarAst;
	}

	[[nodiscard]] auto parse(std::string_view src) -> Parser::Ast
	{
		return Lexer::parse(src, getJsonGrammarAst());
	}

	namespace Literals
	{
		[[nodiscard]] auto operator""_json(const char* cString, std::size_t) -> Parser::Ast
		{
			return parse(cString);
		}
	}
}
