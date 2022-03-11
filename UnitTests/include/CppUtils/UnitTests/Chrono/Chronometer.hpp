#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Chrono::Chronometer
{
	TEST_GROUP("Chrono/Chronometer")
	{
		addTest("", [] {
			auto chrono = CppUtils::Chrono::Chronometer{};
			chrono.stop();
			auto string = CppUtils::Chrono::durationToString(chrono.getDuration());
			ASSERT(!string.empty());
			CppUtils::Log::Logger::logInformation(string);
		});
	}
}
