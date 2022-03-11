#pragma once

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>
#include <CppUtils/Language/Parser/Modifiers.hpp>

namespace CppUtils::Language::Note
{
	class NoteLexer final
	{
	public:
		NoteLexer():
			m_incrementation{0u}
		{
			using namespace std::literals;
			using namespace Type::Literals;
			using namespace std::placeholders;

			m_grammarLexer.addParsingFunction("initIncrementionParser"_token, [this](Parser::Context<Type::Token, std::string>& context) -> bool {
				auto& [cursor, parentNode, firstChildPosition, parsingErrors] = context;
				auto i = 0u;
				while (cursor.isEqualSkipIt("\t"sv))
					++i;
				m_incrementation = i;
				return true;
			});
			m_grammarLexer.addParsingFunction("scopeParser"_token, [this](Parser::Context<Type::Token, std::string>& context) -> bool {
				auto& [cursor, parentNode, firstChildPosition, parsingErrors] = context;
				auto i = 0u;
				while (cursor.isEqualSkipIt("\t"sv))
					++i;
				return (i == m_incrementation);
			});
			m_grammarLexer.addParsingFunction("incrementParser"_token, [this]([[maybe_unused]] Parser::Context<Type::Token, std::string>& context) -> bool {
				++m_incrementation;
				return true;
			});
			m_grammarLexer.addParsingFunction("decrementParser"_token, [this]([[maybe_unused]] Parser::Context<Type::Token, std::string>& context) -> bool {
				--m_incrementation;
				return true;
			});
			m_grammarLexer.addParsingFunction("contentParser"_token, Parser::escapeCharParser<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("trimModifier"_token, Parser::Modifier::trimModifier<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("stringToTokenModifier"_token, std::bind(Parser::Modifier::stringToTokenModifier<Type::Token, std::string>, _1, true));
			m_grammarLexer.addParsingFunction("insertEmptyString"_token, Parser::Modifier::insertEmptyString<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("insertSpace"_token, std::bind(Parser::Modifier::insertString<Type::Token, std::string>, _1, " "sv));
			
			static constexpr auto grammarSrc = R"(
			main: _initIncrementation , (_rootElement >= 0) _spaces;
			!_initIncrementation: (_emptyLine >= 0) , initIncrementionParser;
			!_rootElement: (_node || _value);
			!_element: (_emptyLine >= 0) scopeParser (_node || _value);
			!_node: [_key] _spaces , (_value || _nodeContent);
			!_nodeContent: '\n' incrementParser (_element > 0) decrementParser;
			!_key: ((_char != (':' || '\n')) > 0) trimModifier stringToTokenModifier ':';
			!_value: (_empty || _multilineString || _string) _spaces '\n';
			!_string: insertEmptyString ((_char != '\n') > 0) trimModifier;
			!_multilineString: _spaces '`' insertEmptyString , ((_stringLine != '`') >= 0) '`' trimModifier;
			!_stringLine: _spaces ((_char != ('\n' || '`')) >= 0) trimModifier insertSpace ~'\n';
			!_char: contentParser;
			!_tabs: ~('\n' tabParser) _spaces;
			!_emptyLine: _spaces '\n';
			!_empty: !'_';
			!_spaces: ((' ' || '\t') >= 0);
			)"sv;
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] Parser::ASTNode<Type::Token, std::string> parse(std::string_view src)
		{
			using namespace Type::Literals;
			return m_grammarLexer.parseLanguage("main"_token, src);
		}

	private:
		Lexer::GrammarLexer<Type::Token, std::string> m_grammarLexer;
		std::size_t m_incrementation;
	};

	[[nodiscard]] inline Parser::ASTNode<Type::Token, std::string> parse(std::string_view src)
	{
		static auto noteLexer = NoteLexer{};
		return noteLexer.parse(src);
	}

	namespace Literals
	{
		[[nodiscard]] Parser::ASTNode<Type::Token, std::string> operator"" _note(const char* cstring, std::size_t)
		{
			return parse(cstring);
		}
	}
}
