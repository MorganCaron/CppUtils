#pragma once

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>
#include <CppUtils/Language/Parser/Converters.hpp>

namespace CppUtils::Language::Json
{
	class JsonLexer final
	{
	public:
		JsonLexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;
			using namespace std::placeholders;

			m_grammarLexer.addParsingFunction("spaceParser"_token, Parser::spaceParser<Type::Token, bool, float, std::string>);
			m_grammarLexer.addParsingFunction("booleanParser"_token, Parser::booleanParser<Type::Token, bool, float, std::string>);
			m_grammarLexer.addParsingFunction("floatParser"_token, Parser::floatParser<Type::Token, bool, float, std::string>);
			m_grammarLexer.addParsingFunction("doubleQuoteParser"_token, Parser::doubleQuoteParser<Type::Token, bool, float, std::string>);
			m_grammarLexer.addParsingFunction("stringToTokenConverter"_token, std::bind(Parser::Converter::stringToTokenConverter<Type::Token, bool, float, std::string>, _1, true));

			static constexpr auto grammarSrc = R"(
			main: _object spaceParser;
			!_object: spaceParser '{' ~pairs spaceParser '}';
			!pairs: pair ~nextPair;
			!nextPair: spaceParser ',' pairs;
			!_string: doubleQuoteParser;
			!_key: _string stringToTokenConverter;
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
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] inline Parser::ASTNode<Type::Token, bool, float, std::string> parse(std::string_view src) const
		{
			using namespace Type::Literals;
			return m_grammarLexer.parseLanguage("main"_token, src);
		}

	private:
		Lexer::GrammarLexer<Type::Token, bool, float, std::string> m_grammarLexer;
	};

	[[nodiscard]] inline Parser::ASTNode<Type::Token, bool, float, std::string> parse(std::string_view src)
	{
		static const auto jsonLexer = JsonLexer{};
		return jsonLexer.parse(src);
	}

	namespace Literals
	{
		[[nodiscard]] Parser::ASTNode<Type::Token, bool, float, std::string> operator"" _json(const char* cstring, std::size_t)
		{
			return parse(cstring);
		}
	}
}
