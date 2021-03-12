#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Type::Typed
{
	using namespace CppUtils::Type::Literals;

	constexpr auto IntType = "Int"_token;
	using Int = CppUtils::Type::Typed<IntType, int>;

	constexpr auto StringType = "String"_token;
	using String = CppUtils::Type::Typed<StringType, std::string>;

	TEST_GROUP("Type/Typed")
	{
		using namespace std::literals;
		
		addTest("", [] {
			const auto typedInt = Int{42};
			const auto typedString = String{"text"};

			ASSERT(typedInt.getType() == IntType);
			ASSERT(typedString.getType() == StringType);
			ASSERT(typedInt.getType() != typedString.getType());

			CppUtils::Log::Logger::logInformation(std::to_string(typedInt.value));
			CppUtils::Log::Logger::logInformation(typedString.value);
		});

		addTest("vector", [] {
			auto values = std::vector<std::unique_ptr<CppUtils::Type::ITyped>>{};
			values.emplace_back(std::make_unique<Int>(42));
			values.emplace_back(std::make_unique<String>("text"));

			for (const auto& value : values)
			{
				CppUtils::Log::Logger::logInformation(value->getType().name.data() + ": "s, false);
				if (value->getType() == IntType)
					CppUtils::Log::Logger::logInformation(std::to_string(CppUtils::Type::ensureType<Int>(value).value));
				else if (value->getType() == StringType)
					CppUtils::Log::Logger::logInformation(CppUtils::Type::ensureType<String>(value).value);
			}

			ASSERT(values.at(0)->getType() == IntType);
			ASSERT(values.at(1)->getType() == StringType);
		});
	}
}
