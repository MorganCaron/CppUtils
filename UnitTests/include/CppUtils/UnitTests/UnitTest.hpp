#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest
{
	auto _ = TestSuite{"UnitTest",
		[](auto& suite) {
			suite.addTest("expect", [&] {
				suite.expect(true);
			});

			suite.addTest("expectEqual", [&] {
				using namespace std::literals;

				suite.expectEqual(1, 1);
				suite.expectEqual("test"sv, "test"sv);
			});
		}};
}
