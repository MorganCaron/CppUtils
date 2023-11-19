#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Functional::Function
{
	TEST_GROUP("Functional")
	{
		using Logger = CppUtils::Logger<"CppUtils">;
		
		addTest("callFunction", [] {
			const auto sum = std::function<int(int, int, int)>{[](int a, int b, int c) {
				return a + b + c;
			}};
			const auto vec = std::vector<int>{10, 15, 17};
			const auto result = CppUtils::Functional::callFunction<3>(sum, vec);
			Logger::print(std::to_string(result));
			EXPECT(result == 42);
		});
	}
}
