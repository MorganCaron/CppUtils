#pragma once

#include <CppUtils.hpp>

namespace UnitTests::Type::Index
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("Type/Index", [] {
			const auto numberType0 = CppUtils::Type::TypeId{"Number"};
			const auto numberType1 = CppUtils::Type::TypeId{"Number"};
			const auto stringType = CppUtils::Type::TypeId{"String"};
			ASSERT(numberType0 == numberType1);
			ASSERT(numberType0 != stringType);
		})

	};
}
