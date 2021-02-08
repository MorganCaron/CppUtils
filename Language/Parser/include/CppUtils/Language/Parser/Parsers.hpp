#pragma once

#include <CppUtils/Type/Concepts.hpp>
#include <CppUtils/Language/Parser/Context.hpp>
#include <CppUtils/Language/Parser/Expression.hpp>

namespace CppUtils::Language::Parser
{
	template<typename... Types>
	inline bool spaceParser(Context<Types...>& context)
	{
		context.cursor.skipSpaces();
		return true;
	}

	template<typename... Types> requires Type::Concept::isPresent<Type::Token, Types...> || Type::Concept::isPresent<std::string, Types...>
	[[nodiscard]] inline bool keywordParser(Context<Types...>& context)
	{
		auto& [cursor, parentNode] = context;
		const auto keyword = cursor.getKeywordAndSkipIt();
		if (keyword.empty())
			return false;
		if constexpr(Type::Concept::isPresent<Type::Token, Types...>)
		{
			auto keywordToken = Type::Token{keyword};
			keywordToken.saveTypename();
			parentNode.get().childs.emplace_back(Graph::VariantTreeNode<Types...>{std::move(keywordToken)});
			return true;
		}
		else
			parentNode.get().childs.emplace_back(Graph::VariantTreeNode<Types...>{std::string{keyword}});
		return true;
	}

	template<typename... Types> requires Type::Concept::isPresent<Type::Token, Types...> || Type::Concept::isPresent<std::string, Types...>
	[[nodiscard]] inline bool quoteParser(Context<Types...>& context)
	{
		auto& [cursor, parentNode] = context;
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
			parentNode.get().childs.emplace_back(Graph::VariantTreeNode<Types...>{std::move(stringToken)});
		}
		else
			parentNode.get().childs.emplace_back(Graph::VariantTreeNode<Types...>{std::string{content}});
		++cursor.position;
		return true;
	}

	template<typename... Types> requires Type::Concept::isPresent<Type::Token, Types...> || Type::Concept::isPresent<std::string, Types...>
	[[nodiscard]] inline bool singleQuoteParser(Context<Types...>& context)
	{
		auto& [cursor, parentNode] = context;
		if (cursor.isEndOfString() || cursor.getChar() != '\'')
			return false;
		return quoteParser(context);
	}

	template<typename... Types> requires Type::Concept::isPresent<Type::Token, Types...> || Type::Concept::isPresent<std::string, Types...>
	[[nodiscard]] inline bool doubleQuoteParser(Context<Types...>& context)
	{
		auto& [cursor, parentNode] = context;
		if (cursor.isEndOfString() || cursor.getChar() != '"')
			return false;
		return quoteParser(context);
	}

	template<typename... Types> requires Type::Concept::isPresent<unsigned int, Types...>
	[[nodiscard]] inline bool uintParser(Context<Types...>& context)
	{
		auto& [cursor, parentNode] = context;
		auto string = std::string{""};
		while (!cursor.isEndOfString() && cursor.getChar() >= '0' && cursor.getChar() <= '9')
			string += cursor.getCharAndSkipIt();
		if (string.empty())
			return false;
		const auto number = std::stoul(string);
		if (number != static_cast<unsigned int>(number))
			throw std::out_of_range{"Number too large"};
		parentNode.get().childs.emplace_back(Graph::VariantTreeNode<Types...>{number});
		return true;
	}

	template<typename... Types> requires Type::Concept::isPresent<int, Types...>
	[[nodiscard]] inline bool intParser(Context<Types...>& context)
	{
		auto& [cursor, parentNode] = context;
		if (cursor.isEndOfString())
			return false;
		auto numberLength = std::size_t{};
		const auto number = std::stoi(cursor.src.data(), &numberLength);
		cursor.position += numberLength;
		parentNode.get().childs.emplace_back(Graph::VariantTreeNode<Types...>{number});
		return true;
	}

	template<typename... Types> requires Type::Concept::isPresent<float, Types...>
	[[nodiscard]] inline bool floatParser(Context<Types...>& context)
	{
		auto& [cursor, parentNode] = context;
		if (cursor.isEndOfString())
			return false;
		bool hasDigit = std::isdigit(cursor.getChar());
		bool isInfinity = cursor.isEqual("inf", false);
		if (cursor.getChar() == '+' || cursor.getChar() == '-')
		{
			++cursor.position;
			if (cursor.isEndOfString())
				return false;
			hasDigit |= std::isdigit(cursor.getChar());
			isInfinity |= cursor.isEqual("inf", false);
			--cursor.position;
		}
		if (!hasDigit && !isInfinity)
			return false;
		auto numberLength = std::size_t{};
		const auto number = std::stof(cursor.src.substr(cursor.position).data(), &numberLength);
		cursor.position += numberLength;
		parentNode.get().childs.emplace_back(Graph::VariantTreeNode<Types...>{number});
		return true;
	}

	template<typename... Types> requires Type::Concept::isPresent<bool, Types...>
	[[nodiscard]] inline bool booleanParser(Context<Types...>& context)
	{
		auto& [cursor, parentNode] = context;
		if (cursor.isEqualSkipIt("true", false))
		{
			parentNode.get().childs.emplace_back(Graph::VariantTreeNode<Types...>{true});
			return true;
		}
		if (cursor.isEqualSkipIt("false", false))
		{
			parentNode.get().childs.emplace_back(Graph::VariantTreeNode<Types...>{false});
			return true;
		}
		return false;
	}
}
