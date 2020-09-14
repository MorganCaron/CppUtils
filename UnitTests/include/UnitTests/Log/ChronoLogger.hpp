#pragma once

#include <CppUtils.hpp>

namespace UnitTests::Log::ChronoLogger
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("Log/ChronoLogger", [] {
			auto chronoLogger = CppUtils::Log::ChronoLogger{"Test"};

			chronoLogger.stop();
			ASSERT(true);
		})

	};
}
