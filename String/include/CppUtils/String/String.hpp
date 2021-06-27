#pragma once

#include <cctype>
#include <vector>
#include <string>
#include <numeric>
#include <string_view>

namespace CppUtils::String
{
	[[nodiscard]] inline std::string concatenateStringsWithDelimiter(const std::vector<std::string>& strings, std::string_view delimiter)
	{
		return std::accumulate(strings.begin(), strings.end(), std::string{},
			[delimiter](const std::string& lhs, const std::string& rhs) {
				return lhs.empty() ? rhs : (lhs + delimiter.data() + rhs);
			});
	}
	
	[[nodiscard]] inline std::string concatenateStringsWithDelimiter(const std::vector<std::string_view>& strings, std::string_view delimiter)
	{
		return std::accumulate(strings.begin(), strings.end(), std::string{},
			[delimiter](std::string_view lhs, std::string_view rhs) {
				return lhs.empty() ? std::string{rhs} : (std::string{lhs} + std::string{delimiter} + std::string{rhs});
			});
	}

	template<typename String>
	[[nodiscard]] inline std::vector<String> cstringArrayToVectorOfStrings(const char** cstringArray, std::size_t length)
	{
		if (length == 0)
			return {};
		return std::vector<String>{cstringArray, cstringArray + length};
	}

	[[nodiscard]] inline std::string_view leftTrimString(std::string_view stringView)
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

	[[nodiscard]] inline std::string_view rightTrimString(std::string_view stringView)
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

	[[nodiscard]] inline std::string_view trimString(std::string_view stringView)
	{
		return rightTrimString(leftTrimString(stringView));
	}

	[[nodiscard]] inline std::string getPrintableChar(char c)
	{
		using namespace std::literals;
		return std::isprint(c) ? ("'"s + c + '\'') : std::to_string(c);
	}
}
