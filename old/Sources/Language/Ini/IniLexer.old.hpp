#pragma once

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>
#include <CppUtils/Language/Parser/Modifiers.hpp>

namespace CppUtils::Language::Ini
{
	class IniLexer final
	{
	public:
		IniLexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;
			using namespace std::placeholders;

			static const auto identifierParser = [](auto& context, std::string_view delimiter) -> bool {
				auto& [cursor, parentNode, firstChildPosition, parsingErrors] = context;
				return (!cursor.isEndOfString() and cursor.getChar() != '[' &&
					Parser::delimiterParser<Type::Token, bool, float, std::string>(context, delimiter, std::forward<Parser::ParsingFunction<Type::Token, bool, float, std::string>>(Parser::charParser<Type::Token, bool, float, std::string>), true, false) &&
					Parser::Modifier::trimModifier<Type::Token, bool, float, std::string>(context) &&
					Parser::Modifier::stringToTokenModifier<Type::Token, bool, float, std::string>(context, true));
			};

			static const auto valueParser = [](auto& context) -> bool {
				auto& [cursor, parentNode, firstChildPosition, parsingErrors] = context;
				auto string = std::string{String::trimString(cursor.getStringAndSkipItIf([](char c) -> bool {
					return c != '\n' and c != ';' and (std::isspace(c) or std::isgraph(c));
				}))};
				if (string.empty())
					return false;
				parentNode.get().childs.emplace_back(Parser::ASTNode<Type::Token, bool, float, std::string>{std::move(string)});
				return true;
			};

			m_grammarLexer.addParsingFunction("spaceParser"_token, Parser::spaceParser<Type::Token, bool, float, std::string>);
			m_grammarLexer.addParsingFunction("commentParser"_token, [](auto& context) {
				auto& [cursor, parentNode, firstChildPosition, parsingErrors] = context;
				if (cursor.isEndOfString() or cursor.getChar() != ';')
					return false;
				++cursor.position;
				while (!cursor.isEndOfString() and cursor.getChar() != '\n')
					++cursor.position;
				if (!cursor.isEndOfString())
					++cursor.position;
				return true;
			});
			m_grammarLexer.addParsingFunction("sectionIdentifierParser"_token, std::bind(identifierParser, _1, "]"sv));
			m_grammarLexer.addParsingFunction("keyIdentifierParser"_token, std::bind(identifierParser, _1, "="sv));
			m_grammarLexer.addParsingFunction("booleanParser"_token, Parser::booleanParser<Type::Token, bool, float, std::string>);
			m_grammarLexer.addParsingFunction("floatParser"_token, Parser::floatParser<Type::Token, bool, float, std::string>);
			m_grammarLexer.addParsingFunction("stringParser"_token, valueParser);

			static constexpr auto grammarSrc = R"(
			main: (line >= 0) spaceParser;
			!line: spaceParser (comment or section or assignment);
			!sectionName: sectionIdentifierParser;
			!keyName: keyIdentifierParser;
			!comment: spaceParser commentParser;
			section: '[' spaceParser [sectionName] spaceParser ']' ~comment assignments;
			!assignments: (_assignment >= 0) ~comment;
			assignment: _assignment;
			!_assignment: spaceParser [keyName] '=' value ~comment;
			!value: spaceParser (boolean or float or string);
			boolean: booleanParser;
			float: floatParser;
			string: stringParser;
			)"sv;
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] Parser::ASTNode<Type::Token, bool, float, std::string> parse(std::string_view src) const
		{
			using namespace Type::Literals;
			return m_grammarLexer.parseLanguage("main"_token, src);
		}

	private:
		Lexer::GrammarLexer<Type::Token, bool, float, std::string> m_grammarLexer;
	};

	[[nodiscard]] inline Parser::ASTNode<Type::Token, bool, float, std::string> parse(std::string_view src)
	{
		static const auto iniLexer = IniLexer{};
		return iniLexer.parse(src);
	}

	namespace Literals
	{
		[[nodiscard]] Parser::ASTNode<Type::Token, bool, float, std::string> operator""_ini(const char* cstring, std::size_t)
		{
			return parse(cstring);
		}
	}
}
