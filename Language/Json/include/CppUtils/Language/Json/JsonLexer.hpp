#pragma once

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>

namespace CppUtils::Language::Json
{
	class JsonLexer final
	{
	public:
		JsonLexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;

			m_grammarLexer.addParsingFunction("spaceParser"_token, Parser::spaceParser<Type::Token, bool, float>);
			m_grammarLexer.addParsingFunction("booleanParser"_token, Parser::booleanParser<Type::Token, bool, float>);
			m_grammarLexer.addParsingFunction("floatParser"_token, Parser::floatParser<Type::Token, bool, float>);
			m_grammarLexer.addParsingFunction("doubleQuoteParser"_token, Parser::doubleQuoteParser<Type::Token, bool, float>);

			static constexpr auto grammarSrc = R"(
			main: _object spaceParser;
			!_object: spaceParser '{' ~pairs spaceParser '}';
			!pairs: pair ~nextPair;
			!nextPair: spaceParser ',' pairs;
			!_string: doubleQuoteParser;
			!pair: spaceParser [_string] spaceParser ':' value;
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
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] inline Parser::ASTNode<Type::Token, bool, float> parse(std::string_view src) const
		{
			using namespace Type::Literals;
			return m_grammarLexer.parseLanguage("main"_token, src);
		}

	private:
		Lexer::GrammarLexer<Type::Token, bool, float> m_grammarLexer;
	};

	[[nodiscard]] inline Parser::ASTNode<Type::Token, bool, float> parse(std::string_view src)
	{
		static const auto jsonLexer = JsonLexer{};
		return jsonLexer.parse(src);
	}

	namespace Literals
	{
		[[nodiscard]] Parser::ASTNode<Type::Token, bool, float> operator"" _json(const char* cstring, std::size_t)
		{
			return parse(cstring);
		}
	}
}
