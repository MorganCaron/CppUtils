#pragma once

#include <CppUtils/Type/Traits.hpp>
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

	template<typename... Types> requires Type::Traits::isPresent<Type::Token, Types...> || Type::Traits::isPresent<std::string, Types...>
	[[nodiscard]] inline bool keywordParser(Context<Types...>& context)
	{
		auto& [cursor, parentNode] = context;
		const auto keyword = cursor.getKeywordAndSkipIt();
		if (keyword.empty())
			return false;
		if constexpr(Type::Traits::isPresent<Type::Token, Types...>)
		{
			auto keywordToken = Type::Token{keyword};
			keywordToken.saveTypename();
			parentNode.get().childs.emplace_back(ASTNode<Types...>{std::move(keywordToken)});
			return true;
		}
		else
			parentNode.get().childs.emplace_back(ASTNode<Types...>{std::string{keyword}});
		return true;
	}

	template<typename... Types> requires Type::Traits::isPresent<Type::Token, Types...> || Type::Traits::isPresent<std::string, Types...>
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
		if constexpr(Type::Traits::isPresent<std::string, Types...>)
			parentNode.get().childs.emplace_back(ASTNode<Types...>{std::string{content}});
		else
		{
			auto stringToken = Type::Token{content};
			stringToken.saveTypename();
			parentNode.get().childs.emplace_back(ASTNode<Types...>{std::move(stringToken)});
		}
		++cursor.position;
		return true;
	}

	template<typename... Types> requires Type::Traits::isPresent<Type::Token, Types...> || Type::Traits::isPresent<std::string, Types...>
	[[nodiscard]] inline bool singleQuoteParser(Context<Types...>& context)
	{
		auto& [cursor, parentNode] = context;
		if (cursor.isEndOfString() || cursor.getChar() != '\'')
			return false;
		return quoteParser(context);
	}

	template<typename... Types> requires Type::Traits::isPresent<Type::Token, Types...> || Type::Traits::isPresent<std::string, Types...>
	[[nodiscard]] inline bool doubleQuoteParser(Context<Types...>& context)
	{
		auto& [cursor, parentNode] = context;
		if (cursor.isEndOfString() || cursor.getChar() != '"')
			return false;
		return quoteParser(context);
	}

	template<typename... Types> requires ((std::is_integral_v<Types> && std::is_unsigned_v<Types> && sizeof(Types) >= 4) || ...)
	[[nodiscard]] inline bool uintParser(Context<Types...>& context)
	{
		auto& [cursor, parentNode] = context;
		if (cursor.isEndOfString() || !std::isdigit(cursor.getChar()))
			return false;
		auto numberLength = std::size_t{};
		const auto number = std::stoul(cursor.src.substr(cursor.position).data(), &numberLength);
		if (number != static_cast<unsigned int>(number))
			throw std::out_of_range{"Number too large"};
		cursor.position += numberLength;
		parentNode.get().childs.emplace_back(ASTNode<Types...>{static_cast<unsigned int>(number)});
		return true;
	}

	template<typename... Types> requires ((std::is_integral_v<Types> && std::is_signed_v<Types> && sizeof(Types) >= 4) || ...)
	[[nodiscard]] inline bool intParser(Context<Types...>& context)
	{
		auto& [cursor, parentNode] = context;
		if (cursor.isEndOfString())
			return false;
		bool hasDigit = std::isdigit(cursor.getChar());
		if (cursor.getChar() == '+' || cursor.getChar() == '-')
		{
			++cursor.position;
			if (cursor.isEndOfString())
				return false;
			hasDigit |= static_cast<bool>(std::isdigit(cursor.getChar()));
			--cursor.position;
		}
		if (!hasDigit)
			return false;
		auto numberLength = std::size_t{};
		const auto number = std::stoi(cursor.src.substr(cursor.position).data(), &numberLength);
		cursor.position += numberLength;
		parentNode.get().childs.emplace_back(ASTNode<Types...>{number});
		return true;
	}

	template<typename... Types> requires ((std::is_integral_v<Types> && std::is_unsigned_v<Types> && sizeof(Types) >= 8) || ...)
	[[nodiscard]] inline bool ulongParser(Context<Types...>& context)
	{
		auto& [cursor, parentNode] = context;
		if (cursor.isEndOfString() || !std::isdigit(cursor.getChar()))
			return false;
		auto numberLength = std::size_t{};
		const auto number = std::stoul(cursor.src.substr(cursor.position).data(), &numberLength);
		cursor.position += numberLength;
		parentNode.get().childs.emplace_back(ASTNode<Types...>{number});
		return true;
	}

	template<typename... Types> requires ((std::is_integral_v<Types> && std::is_signed_v<Types> && sizeof(Types) >= 8) || ...)
	[[nodiscard]] inline bool longParser(Context<Types...>& context)
	{
		auto& [cursor, parentNode] = context;
		if (cursor.isEndOfString())
			return false;
		bool hasDigit = std::isdigit(cursor.getChar());
		if (cursor.getChar() == '+' || cursor.getChar() == '-')
		{
			++cursor.position;
			if (cursor.isEndOfString())
				return false;
			hasDigit |= static_cast<bool>(std::isdigit(cursor.getChar()));
			--cursor.position;
		}
		if (!hasDigit)
			return false;
		auto numberLength = std::size_t{};
		const auto number = std::stol(cursor.src.substr(cursor.position).data(), &numberLength);
		cursor.position += numberLength;
		parentNode.get().childs.emplace_back(ASTNode<Types...>{number});
		return true;
	}

	template<typename... Types> requires (std::is_floating_point_v<Types> || ...)
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
			hasDigit |= static_cast<bool>(std::isdigit(cursor.getChar()));
			isInfinity |= cursor.isEqual("inf", false);
			--cursor.position;
		}
		if (!hasDigit && !isInfinity)
			return false;
		auto numberLength = std::size_t{};
		const auto number = std::stof(cursor.src.substr(cursor.position).data(), &numberLength);
		cursor.position += numberLength;
		parentNode.get().childs.emplace_back(ASTNode<Types...>{number});
		return true;
	}

	template<typename... Types> requires Type::Traits::isPresent<bool, Types...>
	[[nodiscard]] inline bool booleanParser(Context<Types...>& context)
	{
		auto& [cursor, parentNode] = context;
		if (cursor.isEqualSkipIt("true", false))
		{
			parentNode.get().childs.emplace_back(ASTNode<Types...>{true});
			return true;
		}
		if (cursor.isEqualSkipIt("false", false))
		{
			parentNode.get().childs.emplace_back(ASTNode<Types...>{false});
			return true;
		}
		return false;
	}
}
