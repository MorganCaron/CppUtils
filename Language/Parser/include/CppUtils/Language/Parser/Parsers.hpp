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
		auto& [cursor, expressionsStack, parentNode] = context;
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

	template<typename... Types> requires Type::Traits::isPresent<std::string, Types...>
	[[nodiscard]] inline bool charParser(Context<Types...>& context)
	{
		auto& [cursor, expressionsStack, parentNode] = context;
		if (cursor.isEndOfString())
			return false;
		const auto c = cursor.getCharAndSkipIt();
		if (parentNode.get().childs.empty() || !std::holds_alternative<std::string>(parentNode.get().childs.back().value))
			parentNode.get().childs.emplace_back(Parser::ASTNode<Types...>{std::string{c}});
		else
			std::get<std::string>(parentNode.get().childs.back().value) += c;
		return true;
	}

	template<typename... Types> requires Type::Traits::isPresent<std::string, Types...>
	[[nodiscard]] inline bool escapeCharParser(Context<Types...>& context)
	{
		auto& [cursor, expressionsStack, parentNode] = context;
		if (cursor.isEndOfString())
			return false;
		auto c = cursor.getCharAndSkipIt();
		if (c == '\\')
		{
			if (cursor.isEndOfString())
				throw std::runtime_error{"An escape character must be followed by another character."};
			c = cursor.getCharAndSkipIt();
			switch (c)
			{
				case '0': c = '\0'; break;
				case 'a': c = '\a'; break;
				case 'b': c = '\b'; break;
				case 'f': c = '\f'; break;
				case 'n': c = '\n'; break;
				case 'r': c = '\r'; break;
				case 't': c = '\t'; break;
				case 'v': c = '\v'; break;
			}
		}
		if (parentNode.get().childs.empty() || !std::holds_alternative<std::string>(parentNode.get().childs.back().value))
			parentNode.get().childs.emplace_back(Parser::ASTNode<Types...>{std::string{c}});
		else
			std::get<std::string>(parentNode.get().childs.back().value) += c;
		return true;
	}

	template<typename... Types>
	[[nodiscard]] inline bool delimiterParser(Context<Types...>& context, std::string_view delimiter, ParsingFunction<Types...> contentParsingFunction, bool multipleContent, bool skipDelimiter)
	{
		using namespace std::literals;
		auto& [cursor, expressionsStack, parentNode] = context;
		parentNode.get().childs.emplace_back(Parser::ASTNode<Types...>{""s});
		auto delimiterFound = (skipDelimiter ? cursor.isEqualSkipIt(delimiter) : cursor.isEqual(delimiter));
		while (!delimiterFound)
		{
			if (cursor.isEndOfString() || !contentParsingFunction(context))
				return false;
			delimiterFound = (skipDelimiter ? cursor.isEqualSkipIt(delimiter) : cursor.isEqual(delimiter));
			if (!multipleContent)
				break;
		}
		return delimiterFound;
	}

	template<typename... Types> requires Type::Traits::isPresent<std::string, Types...>
	[[nodiscard]] inline bool stringParser(Context<Types...>& context, std::string_view delimiter, bool skipDelimiter)
	{
		return delimiterParser<Types...>(context, delimiter, std::forward<ParsingFunction<Types...>>(escapeCharParser<Types...>), true, skipDelimiter);
	}

	template<typename... Types> requires Type::Traits::isPresent<std::string, Types...>
	[[nodiscard]] inline bool quoteParser(Context<Types...>& context)
	{
		using namespace std::literals;
		auto& [cursor, expressionsStack, parentNode] = context;
		if (cursor.isEndOfString())
			return false;
		const auto quote = cursor.getCharAndSkipIt();
		if (quote != '\'' && quote != '"')
			return false;
		return stringParser<Types...>(context, std::string_view{&quote, 1}, true);
	}

	template<typename... Types> requires Type::Traits::isPresent<std::string, Types...>
	[[nodiscard]] inline bool singleQuoteParser(Context<Types...>& context)
	{
		auto& [cursor, expressionsStack, parentNode] = context;
		if (cursor.isEndOfString() || cursor.getChar() != '\'')
			return false;
		return quoteParser(context);
	}

	template<typename... Types> requires Type::Traits::isPresent<std::string, Types...>
	[[nodiscard]] inline bool doubleQuoteParser(Context<Types...>& context)
	{
		auto& [cursor, expressionsStack, parentNode] = context;
		if (cursor.isEndOfString() || cursor.getChar() != '"')
			return false;
		return quoteParser(context);
	}

	template<typename... Types> requires ((std::is_integral_v<Types> && std::is_unsigned_v<Types> && sizeof(Types) >= 4) || ...)
	[[nodiscard]] inline bool uintParser(Context<Types...>& context)
	{
		auto& [cursor, expressionsStack, parentNode] = context;
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
		auto& [cursor, expressionsStack, parentNode] = context;
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
		auto& [cursor, expressionsStack, parentNode] = context;
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
		auto& [cursor, expressionsStack, parentNode] = context;
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
		auto& [cursor, expressionsStack, parentNode] = context;
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
		auto& [cursor, expressionsStack, parentNode] = context;
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
