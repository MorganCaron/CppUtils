#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Log::ChronoLogger
{
	TEST_GROUP("Log/ChronoLogger")
	{
		addTest("", [] {
			auto chronoLogger = CppUtils::Log::ChronoLogger{"Test"};
			chronoLogger.stop();
			TEST_ASSERT(true);
		});
	}
}
