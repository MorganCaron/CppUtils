#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::Functional::Function
{
	auto _ = TestSuite{"Functional", [](auto& suite) {
		using Logger = CppUtils::Logger<"CppUtils">;
		
		suite.addTest("callFunction", [&] {
			const auto sum = std::function<int(int, int, int)>{[](int a, int b, int c) {
				return a + b + c;
			}};
			const auto vec = std::vector<int>{10, 15, 17};
			const auto result = CppUtils::Functional::callFunction<3>(sum, vec);
			Logger::print("{}\n", result);
			suite.expect(result == 42);
		});

	}};
}
