#pragma once

#include <random>
#include <concepts>

namespace CppUtils::Random
{
	template<std::integral Int = std::size_t>
	auto getRandomNumberInInterval(Int min, Int max) -> Int
	{
		static auto randomDevice = std::random_device{};
		static auto engine = std::mt19937{randomDevice()};
		static auto distribution = std::uniform_int_distribution<Int>{min, max};
		auto randomNumber = distribution(engine);
		return randomNumber;
	}
}
