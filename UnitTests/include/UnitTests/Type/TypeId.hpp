#pragma once

#include <CppUtils.hpp>

namespace UnitTests::Type::TypeId
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("Type/TypeId/TypeId", [] {
			const auto numberType0 = CppUtils::Type::TypeId{"Number"};
			const auto numberType1 = CppUtils::Type::TypeId{"Number"};
			const auto stringType = CppUtils::Type::TypeId{"String"};
			ASSERT(numberType0 == numberType1);
			ASSERT(numberType0 != stringType);
		}),

		CppUtils::Test::UnitTest("Type/TypeId/TypeIdStorage", [] {
			auto numberType0 = CppUtils::Type::TypeId{};
			auto numberType1 = CppUtils::Type::TypeId{};
			{
				auto numberTypeName = std::string{"Number"};
				numberType0 = CppUtils::Type::TypeId{numberTypeName};
				numberType1 = CppUtils::Type::TypeId{numberTypeName};
				numberType1.saveTypename();
				numberTypeName = "azerty";
				ASSERT(numberType0 == numberType1);
			}
			auto numberType2 = CppUtils::Type::TypeId{"Number"};
			numberType2.saveTypename();
			
			ASSERT(numberType0 == numberType1);
			ASSERT(numberType1 == numberType2);

			CppUtils::Log::Logger::logInformation(numberType0.name);
			CppUtils::Log::Logger::logInformation(numberType1.name);
			ASSERT(numberType0.name != numberType1.name);
			ASSERT(numberType1.name == numberType2.name);
		})

	};
}
