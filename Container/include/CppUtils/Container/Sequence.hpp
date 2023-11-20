#pragma once

#include <span>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <string_view>

namespace CppUtils::Container::Sequence
{
	enum class Difference: char
	{
		None = 0,
		Added,
		Removed,
		Edited
	};

	template<class T>
	[[nodiscard]] auto getDifferences(std::span<const T> list0, std::span<const T> list1) -> std::vector<Difference>
	{
		auto differences = std::vector<Difference>{};
		differences.reserve(std::max(list0.size(), list1.size()));
		auto i = 0uz, j = 0uz, diffLength = 0uz;
		while (i < list0.size() && j < list1.size())
		{
			const auto& lhs = list0[i];
			const auto& rhs = list1[j];
			if (lhs == rhs)
			{
				if (diffLength > 0)
				{
					for (auto k = 0uz; k < diffLength; ++k)
						differences.emplace_back(Difference::Removed);
					for (; diffLength; --diffLength)
						differences.emplace_back(Difference::Added);
				}
				differences.emplace_back(Difference::None);
			}
			else if (diffLength > 0 && list0[i - diffLength] == rhs)
			{
				i -= diffLength;
				for (; diffLength; --diffLength)
					differences.emplace_back(Difference::Added);
				differences.emplace_back(Difference::None);
			}
			else if (diffLength > 0 && lhs == list1[j - diffLength])
			{
				j -= diffLength;
				for (; diffLength; --diffLength)
					differences.emplace_back(Difference::Removed);
				differences.emplace_back(Difference::None);
			}
			else
				++diffLength;
			++i;
			++j;
		}
		auto offsetList0 = list0.size() - i;
		while (offsetList0--)
			differences.emplace_back(Difference::Removed);
		auto offsetList1 = list1.size() - j;
		while (offsetList1--)
			differences.emplace_back(Difference::Added);
		return differences;
	}
}
