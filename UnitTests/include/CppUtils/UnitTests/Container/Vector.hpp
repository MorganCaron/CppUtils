#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Container::Vector
{
	TEST_GROUP("Container/Vector")
	{
		addTest("merge", [] {
			const auto a = std::vector<std::string>{"a", "b"};
			const auto b = std::vector<std::string>{"c", "d"};
			const auto c = std::vector<std::string>{"e", "f"};
			const auto d = std::vector<std::string>{"g", "h"};
			const auto vector = CppUtils::Container::Vector::merge({std::cref(a), std::cref(b), std::cref(c), std::cref(d)});
			TEST_ASSERT(vector.size() == 8);
			auto logger = CppUtils::Log::Logger{std::cout};
			for (const auto& string : vector)
				logger << string;
			logger << '\n';
		});
	}
}
