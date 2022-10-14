#pragma once

#include <CppUtils/Language/Lexer/Grammar.hpp>

namespace CppUtils::Language::Json
{
	using namespace std::literals;

	constexpr auto jsonGrammarSrc = R"(
		main: object ~spaces;

		# TYPES
		value: ~spaces or(object, string, number, array, boolean, null);
		
		# OBJECT
		object: ~spaces '{' ~repeat(pair, (~spaces ',')) ~spaces '}';
		pair:  hash(key) ~spaces ':' sub(value);
		key: string;

		# STRING
		string: ~spaces or(quote1, quote2);
		quote1: '\'' ~repeat(nonQuote1) '\'';
		quote2: '"' ~repeat(nonQuote2) '"';
		nonQuote1: !'\'' read+;
		nonQuote2: !'"' read+;

		# NUMBER
		number: ~sign repeat(digit) ~decimalPart;
		sign: or(('-' add('-')), ('+' add('+')));
		decimalPart: '.' add('.') repeat(digit);
		digit: [0, 9] read+;
		
		# ARRAY
		array: '[' values ~spaces ']';
		values: repeat(value, (~spaces ','));

		# CONSTANTS
		boolean: or(true, false);
		true: "true" add(true);
		false: "false" add(false);
		null: "null" add(null);

		# SPACES
		spaces: repeat(space);
		space: or(' ', '\n', '\t', '\r');
	)"sv;

	[[nodiscard]] inline auto parse(std::string_view src) -> Parser::Ast
	{
		const auto lowLevelGrammarAst = Parser::parseAst(Lexer::Grammar::lowLevelGrammarSrc);
		const auto highLevelGrammarAst = Lexer::parse(Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
		const auto jsonGrammarAst = Lexer::parse(jsonGrammarSrc, highLevelGrammarAst);
		return Lexer::parse(src, jsonGrammarAst);
	}

	namespace Literals
	{
		[[nodiscard]] inline auto operator"" _json(const char* cString, std::size_t) -> Parser::Ast
		{
			return parse(cString);
		}
	}
}
