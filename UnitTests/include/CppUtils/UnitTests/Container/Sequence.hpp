#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Container::Sequence
{
	[[nodiscard]] static std::vector<CppUtils::Container::Sequence::Difference> generateDifferenceVector(std::string_view string)
	{
		auto result = std::vector<CppUtils::Container::Sequence::Difference>{};
		result.reserve(string.size());
		for (const auto c : string)
		{
			switch (c)
			{
				case ' ': result.emplace_back(CppUtils::Container::Sequence::Difference::None); break;
				case '+': result.emplace_back(CppUtils::Container::Sequence::Difference::Added); break;
				case '-': result.emplace_back(CppUtils::Container::Sequence::Difference::Removed); break;
				case '*': result.emplace_back(CppUtils::Container::Sequence::Difference::Edited); break;
				default: throw std::runtime_error{"generateDifferenceVector(): Unknown character."};
			}
		}
		return result;
	}

	TEST_GROUP("Container/Sequence")
	{
		using namespace std::literals;

		addTest("string", [] {
			const auto s0 = "azTESTerABCtyui99o9p"sv;
			const auto s1 = "000azWXYZerty123uiopHELLO"sv;
			const auto differences = generateDifferenceVector("+++  ----++++  ---  +++  -- - +++++"sv);
			const auto result = CppUtils::Container::Sequence::getDifferences<char>(s0, s1);
			ASSERT(result == differences);
		});

		addTest("vector", [] {
			const auto s0 = "azTESTerABCtyui99o9p"sv;
			const auto s1 = "000azWXYZerty123uiopHELLO"sv;
			const auto vec0 = std::vector<char>{s0.begin(), s0.end()};
			const auto vec1 = std::vector<char>{s1.begin(), s1.end()};
			const auto differences = generateDifferenceVector("+++  ----++++  ---  +++  -- - +++++"sv);
			const auto result = CppUtils::Container::Sequence::getDifferences<char>(vec0, vec1);
			ASSERT(result == differences);
		});
	}
}
