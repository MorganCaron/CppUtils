#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Type::Token
{
	const auto tests = std::vector<CppUtils::Test>{

		CppUtils::Test{"Type/Token/Token", [] {
			using namespace CppUtils::Type::Literals;

			const auto numberType0 = "Number"_token;
			const auto numberType1 = "Number"_token;
			const auto stringType = "String"_token;
			ASSERT(numberType0 == numberType1);
			ASSERT(numberType0 != stringType);

			const auto types = std::unordered_map<CppUtils::Type::Token, std::string, CppUtils::Type::Token::hash_fn>{
				{numberType0, std::string{numberType0.name}},
				{stringType, std::string{stringType.name}}
			};
			CppUtils::Log::Logger::logInformation(types.at(stringType));
			ASSERT(types.at(numberType0) == "Number");
			ASSERT(types.at(stringType) == "String");
		}},

		CppUtils::Test{"Type/Token/TokenStorage", [] {
			using namespace CppUtils::Type::Literals;
			
			auto numberType0 = CppUtils::Type::Token{};
			auto numberType1 = CppUtils::Type::Token{};
			{
				auto numberTypeName = std::string{"Number"};
				numberType0 = CppUtils::Type::Token{numberTypeName};
				numberType1 = CppUtils::Type::Token{numberTypeName};
				numberType1.saveTypename();
				numberTypeName = "azerty";
				ASSERT(numberType0 == numberType1);
			}
			auto numberType2 = "Number"_token;
			numberType2.saveTypename();
			
			ASSERT(numberType0 == numberType1);
			ASSERT(numberType1 == numberType2);

			CppUtils::Log::Logger::logInformation(numberType0.name);
			CppUtils::Log::Logger::logInformation(numberType1.name);
			ASSERT(numberType0.name != numberType1.name);
			ASSERT(numberType1.name == numberType2.name);
		}}

	};
}
