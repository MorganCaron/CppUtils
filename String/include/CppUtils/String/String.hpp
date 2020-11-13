#pragma once

#include <string>
#include <vector>
#include <numeric>

namespace CppUtils::String
{
	[[nodiscard]] inline std::string concatenateStringsWithDelimiter(const std::vector<std::string>& strings, std::string_view delimiter)
	{
		return std::accumulate(strings.begin(), strings.end(), std::string{},
			[delimiter](const std::string& lhs, const std::string& rhs) {
				return lhs.empty() ? rhs : (lhs + delimiter.data() + rhs);
			});
	}

	[[nodiscard]] inline std::vector<std::string> cstringArrayToVectorOfStrings(const char** cstringArray, std::size_t length)
	{
		if (length == 0)
			return std::vector<std::string>{};
		return std::vector<std::string>{cstringArray, cstringArray + length};
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
}
