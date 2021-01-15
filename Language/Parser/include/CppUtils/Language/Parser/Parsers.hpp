#pragma once

#include <CppUtils/Language/Parser/Expression.hpp>

namespace CppUtils::Language::Parser
{
	[[nodiscard]] inline bool spaceParser(Parser::Cursor<std::string>& cursor, [[maybe_unused]] Graph::TokenNode& parentNode)
	{
		cursor.skipSpaces();
		return true;
	}

	[[nodiscard]] inline bool keywordParser(Parser::Cursor<std::string>& cursor, Graph::TokenNode& parentNode)
	{
		const auto keyword = cursor.getKeywordAndSkipIt();
		if (keyword.empty())
			return false;
		auto keywordToken = Type::Token{keyword};
		keywordToken.saveTypename();
		parentNode.childs.emplace_back(Graph::TokenNode{std::move(keywordToken)});
		return true;
	}

	[[nodiscard]] inline bool quoteParser(Parser::Cursor<std::string>& cursor, Graph::TokenNode& parentNode)
	{
		if (cursor.isEndOfString())
			return false;
		const auto quote = cursor.getChar();
		if (quote != '\'' && quote != '"')
			return false;
		const auto startPosition = ++cursor.position;
		while (!cursor.isEndOfString() && cursor.getChar() != quote)
			++cursor.position;
		if (cursor.getChar() != quote)
			return false;
		auto stringToken = Type::Token{cursor.src.substr(startPosition, cursor.position - startPosition)};
		stringToken.saveTypename();
		parentNode.childs.emplace_back(Graph::TokenNode{std::move(stringToken)});
		++cursor.position;
		return true;
	}

	[[nodiscard]] inline bool singleQuoteParser(Parser::Cursor<std::string>& cursor, Graph::TokenNode& parentNode)
	{
		if (cursor.isEndOfString() || cursor.getChar() != '\'')
			return false;
		return quoteParser(cursor, parentNode);
	}

	[[nodiscard]] inline bool doubleQuoteParser(Parser::Cursor<std::string>& cursor, Graph::TokenNode& parentNode)
	{
		if (cursor.isEndOfString() || cursor.getChar() != '"')
			return false;
		return quoteParser(cursor, parentNode);
	}

	[[nodiscard]] inline bool uintParser(Parser::Cursor<std::string>& cursor, Graph::TokenNode& parentNode)
	{
		auto string = std::string{""};
		while (!cursor.isEndOfString() && cursor.getChar() >= '0' && cursor.getChar() <= '9')
			string += cursor.getCharAndSkipIt();
		if (string.empty())
			return false;
		auto stringToken = Type::Token{string};
		stringToken.saveTypename();
		parentNode.childs.emplace_back(Graph::TokenNode{std::move(stringToken)});
		return true;
	}
}
