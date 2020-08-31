#pragma once

#include <CppUtils.hpp>

namespace UnitTests::Test::UnitTest
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("Test/UnitTest", [] {
			ASSERT(true);
		})

	};
}
