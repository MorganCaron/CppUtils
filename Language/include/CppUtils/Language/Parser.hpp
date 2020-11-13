#pragma once

#include <CppUtils/Language/Lexeme.hpp>

namespace CppUtils::Language::Parser
{
	using namespace std::literals;
	
	[[nodiscard]] inline bool spaceParser(Reader::Cursor& cursor, [[maybe_unused]] Lexeme::TokenNode& parentNode)
	{
		cursor.skipSpaces();
		return true;
	}

	[[nodiscard]] inline bool keywordParser(Reader::Cursor& cursor, Lexeme::TokenNode& parentNode)
	{
		const auto keyword = cursor.getKeywordAndSkipIt();
		if (keyword.empty())
			return false;
		auto keywordToken = Lexeme::Token{keyword};
		keywordToken.saveTypename();
		parentNode.childs.emplace_back(Lexeme::TokenNode{std::move(keywordToken)});
		return true;
	}

	[[nodiscard]] inline bool quoteParser(Reader::Cursor& cursor, Lexeme::TokenNode& parentNode)
	{
		if (cursor.isEndOfString())
			return false;
		const auto quote = cursor.getChar();
		if (quote != '\'' && quote != '"')
			return false;
		const auto startPos = ++cursor.pos;
		while (!cursor.isEndOfString() && cursor.getChar() != quote)
			++cursor.pos;
		if (cursor.getChar() != quote)
			return false;
		auto stringToken = Lexeme::Token{cursor.src.substr(startPos, cursor.pos - startPos)};
		stringToken.saveTypename();
		parentNode.childs.emplace_back(Lexeme::TokenNode{std::move(stringToken)});
		++cursor.pos;
		return true;
	}

	[[nodiscard]] inline bool singleQuoteParser(Reader::Cursor& cursor, Lexeme::TokenNode& parentNode)
	{
		if (cursor.isEndOfString() || cursor.getChar() != '\'')
			return false;
		return quoteParser(cursor, parentNode);
	}

	[[nodiscard]] inline bool doubleQuoteParser(Reader::Cursor& cursor, Lexeme::TokenNode& parentNode)
	{
		if (cursor.isEndOfString() || cursor.getChar() != '"')
			return false;
		return quoteParser(cursor, parentNode);
	}

	[[nodiscard]] inline bool uintParser(Reader::Cursor& cursor, Lexeme::TokenNode& parentNode)
	{
		auto string = ""s;
		while (!cursor.isEndOfString() && cursor.getChar() >= '0' && cursor.getChar() <= '9')
			string += cursor.getCharAndSkipIt();
		if (string.empty())
			return false;
		auto stringToken = Lexeme::Token{string};
		stringToken.saveTypename();
		parentNode.childs.emplace_back(Lexeme::TokenNode{std::move(stringToken)});
		return true;
	}
}
