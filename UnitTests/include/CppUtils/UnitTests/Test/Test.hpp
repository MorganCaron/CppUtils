#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Test
{
	const auto tests = std::vector<CppUtils::Test>{

		CppUtils::Test{"Test/Test", [] {
			ASSERT(true);
		}}

	};
}
