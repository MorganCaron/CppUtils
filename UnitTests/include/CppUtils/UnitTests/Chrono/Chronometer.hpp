#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Chrono::Chronometer
{
	TEST_GROUP("Chrono/Chronometer")
	{
		addTest("", [] {
			auto chrono = CppUtils::Chrono::Chronometer{};
			chrono.stop();
			ASSERT(!chrono.getText().empty());
			CppUtils::Log::Logger::logInformation(chrono.getText());
		});
	}
}
