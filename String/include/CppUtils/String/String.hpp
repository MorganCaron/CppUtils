#pragma once

#include <cctype>
#include <vector>
#include <string>
#include <numeric>
#include <string_view>
#include <unordered_map>

#include <CppUtils/Type/Concept.hpp>

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

	[[nodiscard]] inline auto concatenateStringsWithDelimiter(const std::vector<std::string>& strings, std::string_view delimiter) -> std::string
	{
		return std::accumulate(strings.begin(), strings.end(), std::string{},
			[delimiter](const std::string& lhs, const std::string& rhs) {
				return lhs.empty() ? rhs : (lhs + delimiter.data() + rhs);
			});
	}
	
	[[nodiscard]] inline auto concatenateStringsWithDelimiter(const std::vector<std::string_view>& strings, std::string_view delimiter) -> std::string
	{
		return std::accumulate(strings.begin(), strings.end(), std::string{},
			[delimiter](std::string_view lhs, std::string_view rhs) {
				return lhs.empty() ? std::string{rhs} : (std::string{lhs} + std::string{delimiter} + std::string{rhs});
			});
	}

	template<Type::Concept::String String>
	[[nodiscard]] auto cstringArrayToVectorOfStrings(const char** cstringArray, std::size_t length) -> std::vector<String>
	{
		if (length == 0)
			return {};
		return std::vector<String>{cstringArray, cstringArray + length};
	}

	[[nodiscard]] inline auto leftTrimString(std::string_view stringView) -> std::string_view
	{
		stringView.remove_prefix(std::distance(stringView.cbegin(),
			std::find_if(stringView.cbegin(), stringView.cend(),
        		[](char c) {
					return !std::isspace(c);
				}
			)
		));
		return stringView;
	}

	[[nodiscard]] inline auto rightTrimString(std::string_view stringView) -> std::string_view
	{
		stringView.remove_suffix(std::distance(stringView.crbegin(),
			std::find_if(stringView.crbegin(), stringView.crend(),
        		[](char c) {
					return !std::isspace(c);
				}
			)
		));
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

	[[nodiscard]] inline auto toLower(std::string_view originalString) -> std::string
	{
		auto result = std::string{originalString};
		for (auto& c : result)
			c = std::tolower(c);
		return result;
	}

	[[nodiscard]] inline auto toUpper(std::string_view originalString) -> std::string
	{
		auto result = std::string{originalString};
		for (auto& c : result)
			c = std::toupper(c);
		return result;
	}
}
