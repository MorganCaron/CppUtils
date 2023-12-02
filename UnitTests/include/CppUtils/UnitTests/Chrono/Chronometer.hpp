#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::Chrono::Chronometer
{
	auto _ = TestSuite{"Chrono", [](auto& suite) {
		using Logger = CppUtils::Logger<"CppUtils">;

		suite.addTest("Chronometer", [&] {
			auto chrono = CppUtils::Chrono::Chronometer{};
			chrono.stop();
			auto string = CppUtils::Chrono::durationToString(chrono.getDuration());
			suite.expect(!std::empty(string));
			Logger::print("{}", string);
		});

	}};
}
