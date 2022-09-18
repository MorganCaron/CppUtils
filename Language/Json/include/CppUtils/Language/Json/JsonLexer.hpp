#pragma once

#include <CppUtils/Language/Lexer/GrammarManager.hpp>

namespace CppUtils::Language::Json
{
	using namespace std::literals;

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
		static auto grammarManager = Lexer::Grammar::GrammarManager{};
		grammarManager.addGrammar("jsonGrammar"sv, jsonGrammar);
		return grammarManager.parseLanguage(src, "jsonGrammar"sv);
	}

	namespace Literals
	{
		[[nodiscard]] inline auto operator"" _json(const char* cString, std::size_t) -> Parser::Ast
		{
			return parse(cString);
		}
	}
}
