#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Switch
{
	TEST_GROUP("Switch")
	{
		using namespace Type::Literals;

		addTest("", [] {
			const auto id0 = CppUtils::Switch::newId(false);
			const auto id1 = CppUtils::Switch::newId(true);
			const auto id2 = CppUtils::Switch::newId(false);
			CppUtils::Switch::setValue(id2, true);
			ASSERT(CppUtils::Switch::getValue(id0) == false);
			ASSERT(CppUtils::Switch::getValue(id1) == true);
			ASSERT(CppUtils::Switch::getValue(id2) == true);
			CppUtils::Switch::deleteId(id0);
			CppUtils::Switch::deleteId(id1);
			CppUtils::Switch::deleteId(id2);
		});

		addTest("Tag", [] {
			const auto id0 = CppUtils::Switch::newId(false);
			const auto id1 = CppUtils::Switch::newId(false, std::array{ "Tag0"_token });
			const auto id2 = CppUtils::Switch::newId(false, std::array{ "Tag1"_token });
			CppUtils::Switch::setValue("Tag0"_token, true);
			ASSERT(CppUtils::Switch::getValue(id0) == false);
			ASSERT(CppUtils::Switch::getValue(id1) == true);
			ASSERT(CppUtils::Switch::getValue(id2) == false);
			CppUtils::Switch::deleteId(id0);
			CppUtils::Switch::deleteId(id1);
			CppUtils::Switch::deleteId(id2);
		});
	}
}
