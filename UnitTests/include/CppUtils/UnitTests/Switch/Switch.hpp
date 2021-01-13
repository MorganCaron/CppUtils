#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Switch
{
	const auto tests = std::vector<CppUtils::Test>{

		CppUtils::Test{"Switch", [] {
			const auto id0 = CppUtils::Switch::newId(false);
			const auto id1 = CppUtils::Switch::newId(true);
			const auto id2 = CppUtils::Switch::newId(false);
			CppUtils::Switch::setValue(id2, true);

			ASSERT(CppUtils::Switch::getValue(id0) == false);
			ASSERT(CppUtils::Switch::getValue(id1) == true);
			ASSERT(CppUtils::Switch::getValue(id2) == true);
		}},

		CppUtils::Test{"Switch/Tag", [] {
			using namespace Type::Literals;

			const auto id0 = CppUtils::Switch::newId(false);
			const auto id1 = CppUtils::Switch::newId(false, std::array{ "Tag0"_token });
			const auto id2 = CppUtils::Switch::newId(false, std::array{ "Tag1"_token });
			CppUtils::Switch::setValue("Tag0"_token, true);

			ASSERT(CppUtils::Switch::getValue(id0) == false);
			ASSERT(CppUtils::Switch::getValue(id1) == true);
			ASSERT(CppUtils::Switch::getValue(id2) == false);
		}}

	};
}
