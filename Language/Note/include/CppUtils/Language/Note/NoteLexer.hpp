#pragma once

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>
#include <CppUtils/Language/Parser/Modifiers.hpp>

namespace CppUtils::Language::Note
{
	class NoteLexer final
	{
	public:
		NoteLexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;
			using namespace std::placeholders;

			m_grammarLexer.addParsingFunction("spaceParser"_token, Parser::spaceParser<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("contentParser"_token, Parser::escapeCharParser<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("trimModifier"_token, Parser::Modifier::trimModifier<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("stringToTokenModifier"_token, std::bind(Parser::Modifier::stringToTokenModifier<Type::Token, std::string>, _1, true));
			m_grammarLexer.addParsingFunction("insertSpace"_token, std::bind(Parser::Modifier::insertString<Type::Token, std::string>, _1, " "sv));
			
			static constexpr auto grammarSrc = R"(
			main: _element spaceParser;
			!_element: ((_node > 0) || _value) spaceParser;
			!_node: [_key] spaceParser _element;
			!_key: spaceParser ((_char != (':' || '\n')) > 0) trimModifier stringToTokenModifier ':';
			!_value: (_empty || _multilineString || _string);
			!_string: spaceParser ((_char != '\n') > 0) trimModifier '\n';
			!_multilineString: spaceParser '`' (_stringLine >= 0) trimModifier spaceParser '`';
			!_stringLine: spaceParser ((_char != ('\n' || '`')) > 0) trimModifier insertSpace;
			!_char: contentParser;
			!_empty: spaceParser "_\n";
			)"sv;
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] inline Parser::ASTNode<Type::Token, std::string> parse(std::string_view src) const
		{
			using namespace Type::Literals;
			return m_grammarLexer.parseLanguage("main"_token, src);
		}

	private:
		Lexer::GrammarLexer<Type::Token, std::string> m_grammarLexer;
	};

	[[nodiscard]] inline Parser::ASTNode<Type::Token, std::string> parse(std::string_view src)
	{
		static const auto noteLexer = NoteLexer{};
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
