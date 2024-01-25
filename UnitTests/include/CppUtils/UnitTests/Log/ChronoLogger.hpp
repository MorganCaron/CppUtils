#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::Log::ChronoLogger
{
	auto _ = TestSuite{"Log", [](auto& suite) {
		suite.addTest("ChronoLogger", [&] {
			auto chronoLogger = CppUtils::Log::ChronoLogger{"Test"};
			chronoLogger.stop();
			suite.expect(true);
		});
	}};
}
