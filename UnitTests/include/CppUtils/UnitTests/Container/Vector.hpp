#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::Container::Vector
{
	auto _ = TestSuite{"Container/Vector", [](auto& suite) {
		using Logger = CppUtils::Logger<"CppUtils">;

		suite.addTest("merge", [&] {
			const auto a = std::vector<std::string>{"a", "b"};
			const auto b = std::vector<std::string>{"c", "d"};
			const auto c = std::vector<std::string>{"e", "f"};
			const auto d = std::vector<std::string>{"g", "h"};
			const auto vector = CppUtils::Container::Vector::merge({std::cref(a), std::cref(b), std::cref(c), std::cref(d)});
			suite.expectEqual(std::size(vector), 8uz);
			for (const auto& string : vector)
				Logger::print("{}", string);
		});

	}};
}
