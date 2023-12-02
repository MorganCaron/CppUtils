#pragma once

#include <cctype>
#include <vector>
#include <string>
#include <numeric>
#include <string_view>
#include <unordered_map>

#include <CppUtils/String/Concept.hpp>

namespace CppUtils::String
{
	const auto escapedChars = std::unordered_map<char, char>{
		{ '0', '\0' },
		{ 'a', '\a' },
		{ 'b', '\b' },
		{ 'f', '\f' },
		{ 'n', '\n' },
		{ 'r', '\r' },
		{ 't', '\t' },
		{ 'v', '\v' }
	};

	template<Concept::Char CharT>
	[[nodiscard]] auto reverseEscapedChar(CharT c) noexcept -> std::basic_string<CharT>
	{
		for (const auto& [readableChar, escapedChar] : String::escapedChars)
			if (escapedChar == c)
				return std::basic_string<CharT>{"\\"} + readableChar;
		return std::basic_string<CharT>{c};
	}

	[[nodiscard]] inline auto concatenateStringsWithDelimiter(const std::vector<std::string>& strings, std::string_view delimiter) -> std::string
	{
		return std::accumulate(std::cbegin(strings), std::cend(strings), std::string{},
			[delimiter](const std::string& lhs, const std::string& rhs) {
				return lhs.empty() ? rhs : (lhs + delimiter.data() + rhs);
			});
	}
	
	[[nodiscard]] inline auto concatenateStringsWithDelimiter(const std::vector<std::string_view>& strings, std::string_view delimiter) -> std::string
	{
		return std::accumulate(std::cbegin(strings), std::cend(strings), std::string{},
			[delimiter](std::string_view lhs, std::string_view rhs) {
				return lhs.empty() ? std::string{rhs} : (std::string{lhs} + std::string{delimiter} + std::string{rhs});
			});
	}

	[[nodiscard]] inline auto leftTrimString(std::string_view stringView) -> std::string_view
	{
		auto prefixLength = static_cast<std::size_t>(std::distance(std::cbegin(stringView),
			std::find_if(std::cbegin(stringView), std::cend(stringView),
				[](char c) {
					return !std::isspace(c);
				}
			)
		));
		stringView.remove_prefix(prefixLength);
		return stringView;
	}

	[[nodiscard]] inline auto rightTrimString(std::string_view stringView) -> std::string_view
	{
		auto suffixLength = static_cast<std::size_t>(std::distance(std::crbegin(stringView),
			std::find_if(std::crbegin(stringView), std::crend(stringView),
				[](char c) {
					return !std::isspace(c);
				}
			)
		));
		stringView.remove_suffix(suffixLength);
		return stringView;
	}

	[[nodiscard]] inline auto trimString(std::string_view stringView) -> std::string_view
	{
		return rightTrimString(leftTrimString(stringView));
	}

	[[nodiscard]] inline auto getPrintableChar(char c) -> std::string
	{
		using namespace std::literals;
		return std::isprint(c) ? ("'"s + c + '\'') : std::to_string(c);
	}

	[[nodiscard]] constexpr auto isUppercase(auto c) noexcept -> bool
	{
		return (c >= 'A' && c <= 'Z');
	}

	[[nodiscard]] constexpr auto isLowercase(auto c) noexcept -> bool
	{
		return (c >= 'a' && c <= 'z');
	}

	[[nodiscard]] constexpr auto toLowercase(auto c) noexcept
	{
		return isUppercase(c) ? c + ('a' - 'A') : c;
	}

	[[nodiscard]] constexpr auto toUppercase(auto c) noexcept
	{
		return isLowercase(c) ? c - ('a' - 'A') : c;
	}

	template<class CharT>
	[[nodiscard]] constexpr auto toLowercase(std::basic_string_view<CharT> originalString) -> std::basic_string<CharT>
	{
		auto result = std::basic_string<CharT>{originalString};
		for (auto& c : result)
			c = toLowercase(c);
		return result;
	}

	template<class CharT>
	[[nodiscard]] constexpr auto toUppercase(std::basic_string_view<CharT> originalString) -> std::basic_string<CharT>
	{
		auto result = std::basic_string<CharT>{originalString};
		for (auto& c : result)
			c = toUppercase(c);
		return result;
	}
	
	[[nodiscard]] constexpr auto formatValue(auto value)
	{
		if constexpr (std::formattable<decltype(value), char>)
			return std::format("{}", value);
		else
			return "<non printable>";
	}
}
