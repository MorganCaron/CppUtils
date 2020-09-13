#pragma once

#include <CppUtils.hpp>

namespace UnitTests::Switch
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("Switch", [] {
			const auto id0 = CppUtils::Switch::newId();
			const auto id1 = CppUtils::Switch::newId(true);
			const auto id2 = CppUtils::Switch::newId();
			CppUtils::Switch::enable(id2);

			ASSERT(CppUtils::Switch::isEnabled(id0) == false);
			ASSERT(CppUtils::Switch::isEnabled(id1) == true);
			ASSERT(CppUtils::Switch::isEnabled(id2) == true);

			const auto id3 = CppUtils::Switch::newId();
			const auto id4 = CppUtils::Switch::newId();
			const auto id5 = CppUtils::Switch::newId();
			CppUtils::Switch::enable(id3, id4, id5);
			CppUtils::Switch::disable(id4, id5);

			ASSERT(CppUtils::Switch::isEnabled(id3) == true);
			ASSERT(CppUtils::Switch::isEnabled(id4) == false);
			ASSERT(CppUtils::Switch::isEnabled(id5) == false);
		})

	};
}
