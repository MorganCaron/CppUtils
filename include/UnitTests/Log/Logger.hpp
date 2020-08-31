#pragma once

#include <CppUtils.hpp>

namespace UnitTests::Log::Logger
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("Log/Logger", [] {
			CppUtils::Logger::logInformation("Information message");
			CppUtils::Logger::logImportant("Important message");
			CppUtils::Logger::logSuccess("Success message");
			CppUtils::Logger::logDebug("Debug message");
			CppUtils::Logger::logWarning("Warning message");
			CppUtils::Logger::logError("Error message");
			ASSERT(true);
		})

	};
}
