#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Chrono::Chronometer
{
	const auto tests = std::vector<CppUtils::Test>{

		CppUtils::Test{"Chrono/Chronometer", [] {
			auto chrono = CppUtils::Chrono::Chronometer{};

			chrono.stop();
			ASSERT(!chrono.getText().empty());
			CppUtils::Log::Logger::logInformation(chrono.getText());
		}}

	};
}
