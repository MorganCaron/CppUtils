#pragma once

#include <CppUtils/Language/Lexer/Grammar.hpp>

namespace CppUtils::Language::Json
{
	using namespace std::literals;

	constexpr auto jsonGrammarSrc = R"(
		main: object ~spaces;
		object: ~spaces '{' ~pairs ~spaces '}';
		pairs: pair ~nextPair;
		nextPair: ~spaces ',' pairs;
		pair: ~spaces key ~spaces ':' value;
		value: ~spaces or(string, number, object, array, boolean, null);

		string: quote;
		number: (digit >= 1) ~decimalPart;
		decimalPart: '.' (digit >= 1);
		array: '[' values ~spaces ']';
		values: value ~nextValue;
		nextValue: ~spaces ',' values;
		boolean: or(true, false);
		true: or('true', '1');
		false: or('false', '0');

		quote: or(quote1, quote2);
		quote1: '\'' nonQuote1 '\'';
		quote2: '"' nonQuote2 '"';
		nonQuote1: !'\'' read+ ~nonQuote1;
		nonQuote2: !'"' read+ ~nonQuote2;

		spaces: space ~spaces;
		space: or(' ', '\n', '\t', '\r');
	)"sv;

	constexpr auto jsonGrammar = R"(
		main: _object spaceParser;
		!_object: spaceParser '{' ~pairs spaceParser '}';
		!pairs: pair ~nextPair;
		!nextPair: spaceParser ',' pairs;
		!_string: doubleQuoteParser;
		!_key: _string stringToTokenModifier;
		!pair: spaceParser [_key] spaceParser ':' value;
		!string: _string;
		!object: _object;
		!value: spaceParser (string || number || object || array || boolean || null);
		!number: floatParser;
		
		!array: '[' values spaceParser ']';
		!values: value ~nextValue;
		!nextValue: spaceParser ',' values;
		!boolean: booleanParser;
		null: "null";
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
