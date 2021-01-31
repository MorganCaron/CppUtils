#pragma once

#include <CppUtils/Type/Concepts.hpp>
#include <CppUtils/Language/Parser/Expression.hpp>

namespace CppUtils::Language::Parser
{
	template<typename... Types>
	inline bool spaceParser(Parser::Cursor<std::string>& cursor, [[maybe_unused]] Graph::VariantTreeNode<Types...>& parentNode)
	{
		cursor.skipSpaces();
		return true;
	}

	template<typename... Types> requires Type::Concept::isPresent<Type::Token, Types...> || Type::Concept::isPresent<std::string, Types...>
	[[nodiscard]] inline bool keywordParser(Parser::Cursor<std::string>& cursor, Graph::VariantTreeNode<Types...>& parentNode)
	{
		const auto keyword = cursor.getKeywordAndSkipIt();
		if (keyword.empty())
			return false;
		if constexpr(Type::Concept::isPresent<Type::Token, Types...>)
		{
			auto keywordToken = Type::Token{keyword};
			keywordToken.saveTypename();
			parentNode.childs.emplace_back(Graph::VariantTreeNode<Types...>{std::move(keywordToken)});
			return true;
		}
		else
			parentNode.childs.emplace_back(Graph::VariantTreeNode<Types...>{std::string{keyword}});
		return true;
	}

	template<typename... Types> requires Type::Concept::isPresent<Type::Token, Types...> || Type::Concept::isPresent<std::string, Types...>
	[[nodiscard]] inline bool quoteParser(Parser::Cursor<std::string>& cursor, Graph::VariantTreeNode<Types...>& parentNode)
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
		const auto content = cursor.src.substr(startPosition, cursor.position - startPosition);
		if constexpr(Type::Concept::isPresent<Type::Token, Types...>)
		{
			auto stringToken = Type::Token{content};
			stringToken.saveTypename();
			parentNode.childs.emplace_back(Graph::VariantTreeNode<Types...>{std::move(stringToken)});
		}
		else
			parentNode.childs.emplace_back(Graph::VariantTreeNode<Types...>{std::string{content}});
		++cursor.position;
		return true;
	}

	template<typename... Types> requires Type::Concept::isPresent<Type::Token, Types...> || Type::Concept::isPresent<std::string, Types...>
	[[nodiscard]] inline bool singleQuoteParser(Parser::Cursor<std::string>& cursor, Graph::VariantTreeNode<Types...>& parentNode)
	{
		if (cursor.isEndOfString() || cursor.getChar() != '\'')
			return false;
		return quoteParser(cursor, parentNode);
	}

	template<typename... Types> requires Type::Concept::isPresent<Type::Token, Types...> || Type::Concept::isPresent<std::string, Types...>
	[[nodiscard]] inline bool doubleQuoteParser(Parser::Cursor<std::string>& cursor, Graph::VariantTreeNode<Types...>& parentNode)
	{
		if (cursor.isEndOfString() || cursor.getChar() != '"')
			return false;
		return quoteParser(cursor, parentNode);
	}

	template<typename... Types> requires Type::Concept::isPresent<unsigned int, Types...>
	[[nodiscard]] inline bool uintParser(Parser::Cursor<std::string>& cursor, Graph::VariantTreeNode<Types...>& parentNode)
	{
		auto string = std::string{""};
		while (!cursor.isEndOfString() && cursor.getChar() >= '0' && cursor.getChar() <= '9')
			string += cursor.getCharAndSkipIt();
		if (string.empty())
			return false;
		const auto number = std::stoul(string);
		if (number != static_cast<unsigned int>(number))
			throw std::out_of_range{"Number too large"};
		parentNode.childs.emplace_back(Graph::VariantTreeNode<Types...>{number});
		return true;
	}
}
