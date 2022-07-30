#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::UnitTest
{
	TEST_GROUP("UnitTest")
	{
		addTest("Test", [] {
			TEST_ASSERT(true);
		});
	}
}
