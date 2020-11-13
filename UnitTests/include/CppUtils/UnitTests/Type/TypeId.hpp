#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Type::TypeId
{
	using namespace CppUtils::Type::Literals;

	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("Type/TypeId/TypeId", [] {
			const auto numberType0 = "Number"_typeId;
			const auto numberType1 = "Number"_typeId;
			const auto stringType = "String"_typeId;
			ASSERT(numberType0 == numberType1);
			ASSERT(numberType0 != stringType);

			const auto types = std::unordered_map<CppUtils::Type::TypeId, std::string, CppUtils::Type::TypeId::hash_fn>{
				{numberType0, std::string{numberType0.name}},
				{stringType, std::string{stringType.name}}
			};
			CppUtils::Log::Logger::logInformation(types.at(stringType));
			ASSERT(types.at(numberType0) == "Number");
			ASSERT(types.at(stringType) == "String");
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
			auto numberType2 = "Number"_typeId;
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
