#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Hash::Token
{
	TEST_GROUP("Hash/Token")
	{
		using namespace CppUtils::Hash::Literals;

		addTest("Token", [] {
			const auto numberType0 = "Number"_token;
			const auto numberType1 = "Number"_token;
			const auto stringType = "String"_token;
			EXPECT(numberType0 == numberType1);
			EXPECT(numberType0 != stringType);
		});
	}
}
