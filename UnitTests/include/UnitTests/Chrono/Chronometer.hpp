#pragma once

#include <CppUtils.hpp>

namespace UnitTests::Chrono::Chronometer
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("Chrono/Chronometer", [] {
			auto chrono = CppUtils::Chrono::Chronometer{};

			chrono.stop();
			ASSERT(!chrono.getText().empty());
			CppUtils::Log::Logger::logInformation(chrono.getText());
		})

	};
}
