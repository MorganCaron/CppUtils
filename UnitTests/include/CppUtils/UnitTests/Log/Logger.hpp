#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Log::Logger
{
	const auto tests = std::vector<CppUtils::Test>{

		CppUtils::Test{"Log/Logger", [] {
			CppUtils::Log::Logger::logInformation("Information message");
			CppUtils::Log::Logger::logImportant("Important message");
			CppUtils::Log::Logger::logSuccess("Success message");
			CppUtils::Log::Logger::logDebug("Debug message");
			CppUtils::Log::Logger::logDetail("Detail message");
			CppUtils::Log::Logger::logWarning("Warning message");
			CppUtils::Log::Logger::logError("Error message");
			ASSERT(true);
		}}

	};
}
