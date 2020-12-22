#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Log::ChronoLogger
{
	const auto tests = std::vector<CppUtils::Test>{

		CppUtils::Test{"Log/ChronoLogger", [] {
			auto chronoLogger = CppUtils::Log::ChronoLogger{"Test"};

			chronoLogger.stop();
			ASSERT(true);
		}}

	};
}
