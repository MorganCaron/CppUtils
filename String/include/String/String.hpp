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
}
