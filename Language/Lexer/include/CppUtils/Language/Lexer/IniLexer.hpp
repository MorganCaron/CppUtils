#pragma once

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>

namespace CppUtils::Language::Lexer
{
	class IniLexer final
	{
	public:
		IniLexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;
			using namespace std::placeholders;

			static const auto identifierParser = [](auto& context, char delimiter) -> bool {
				auto& [cursor, parentNode] = context;
				if (cursor.isEndOfString() || cursor.getChar() == ';' || cursor.getChar() == delimiter)
					return false;
				const auto string = cursor.getCharAndSkipIt() + std::string{String::trimString(cursor.getStringAndSkipItIf([&delimiter](char c) -> bool {
					return c != ';' && c != delimiter;
				}))};
				auto stringToken = Type::Token{string};
				stringToken.saveTypename();
				parentNode.get().childs.emplace_back(Graph::VariantTreeNode<Type::Token, bool, float>{std::move(stringToken)});
				return true;
			};

			m_grammarLexer.addParsingFunction("spaceParser"_token, Parser::spaceParser<Type::Token, bool, float>);
			m_grammarLexer.addParsingFunction("commentParser"_token, [](auto& context) {
				auto& [cursor, parentNode] = context;
				if (cursor.isEndOfString() || cursor.getChar() != ';')
					return false;
				++cursor.position;
				while (!cursor.isEndOfString() && cursor.getChar() != '\n')
					++cursor.position;
				if (!cursor.isEndOfString())
					++cursor.position;
				return true;
			});
			m_grammarLexer.addParsingFunction("sectionIdentifierParser"_token, std::bind(identifierParser, _1, ']'));
			m_grammarLexer.addParsingFunction("keyIdentifierParser"_token, std::bind(identifierParser, _1, '='));
			m_grammarLexer.addParsingFunction("booleanParser"_token, Parser::booleanParser<Type::Token, bool, float>);
			m_grammarLexer.addParsingFunction("floatParser"_token, Parser::floatParser<Type::Token, bool, float>);
			m_grammarLexer.addParsingFunction("stringParser"_token, std::bind(identifierParser, _1, '\n'));

			static constexpr auto grammarSrc = R"(
			main: (_line >= 0) spaceParser;
			_line: spaceParser (_comment || section || assignment);
			_comment: spaceParser commentParser;
			section: '[' spaceParser [sectionIdentifierParser] spaceParser ']' ~_comment _assignments;
			_assignments: (_assignment >= 0) ~_comment;
			assignment: _assignment;
			_assignment: spaceParser [keyIdentifierParser] '=' _value ~_comment;
			_value: spaceParser (boolean || float || string);
			boolean: booleanParser;
			float: floatParser;
			string: stringParser;
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

	namespace Ini
	{
		[[nodiscard]] inline Graph::VariantTreeNode<Type::Token, bool, float> parse(const std::string_view src)
		{
			static const auto iniLexer = CppUtils::Language::Lexer::IniLexer{};
			return iniLexer.parse(src);
		}
	}

	namespace Literals
	{
		[[nodiscard]] Graph::VariantTreeNode<Type::Token, bool, float> operator"" _ini(const char* cstring, std::size_t) noexcept
		{
			return Ini::parse(cstring);
		}
	}
}
