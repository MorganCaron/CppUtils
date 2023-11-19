#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Chrono::Chronometer
{
	TEST_GROUP("Chrono/Chronometer")
	{
		using Logger = CppUtils::Logger<"CppUtils">;

		addTest("", [] {
			auto chrono = CppUtils::Chrono::Chronometer{};
			chrono.stop();
			auto string = CppUtils::Chrono::durationToString(chrono.getDuration());
			EXPECT(!string.empty());
			Logger::print(string);
		});
	}
}
