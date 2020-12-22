#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Functional::Function
{
	const auto tests = std::vector<CppUtils::Test>{

		CppUtils::Test{"Functional/callFunction", [] {
			const auto sum = std::function<int(int, int, int)>{[](int a, int b, int c) {
				return a + b + c;
			}};
			const auto vec = std::vector<int>{10, 15, 17};
			const auto result = CppUtils::Functional::callFunction<3>(sum, vec);
			CppUtils::Log::Logger::logInformation(std::to_string(result));
			ASSERT(result == 42);
		}}

	};
}
