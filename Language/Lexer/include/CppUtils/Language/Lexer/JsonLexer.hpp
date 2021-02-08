#pragma once

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>

namespace CppUtils::Language::Lexer
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
			_object: spaceParser '{' ~_pairs spaceParser '}';
			_pairs: _pair ~_nextPair;
			_nextPair: spaceParser ',' _pairs;
			_pair: spaceParser [doubleQuoteParser] spaceParser ':' _value;
			_value: spaceParser (string || number || object || array || boolean || null);
			string: doubleQuoteParser;
			number: floatParser;
			object: _object;
			array: '[' _values spaceParser ']';
			_values: _value ~_nextValue;
			_nextValue: spaceParser ',' _values;
			boolean: booleanParser;
			null: "null";
			)"sv;
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] inline Graph::VariantTreeNode<Type::Token, bool, float> parse(const std::string_view src) const
		{
			using namespace Type::Literals;
			return m_grammarLexer.parseLanguage("main"_token, src);
		}

	private:
		GrammarLexer<Type::Token, bool, float> m_grammarLexer;
	};

	namespace Json
	{
		[[nodiscard]] inline Graph::VariantTreeNode<Type::Token, bool, float> parse(const std::string_view src)
		{
			static const auto jsonLexer = CppUtils::Language::Lexer::JsonLexer{};
			return jsonLexer.parse(src);
		}
	}

	namespace Literals
	{
		[[nodiscard]] Graph::VariantTreeNode<Type::Token, bool, float> operator"" _json(const char* cstring, std::size_t)
		{
			return Json::parse(cstring);
		}
	}
}
