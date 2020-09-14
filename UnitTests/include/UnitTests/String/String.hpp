#pragma once

#include <CppUtils.hpp>

namespace UnitTests::String
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("String/concatenateStringsWithDelimiter", [] {
			const auto strings = std::vector{"A"s, "B"s, "C"s};
			const auto string = CppUtils::String::concatenateStringsWithDelimiter(strings, ", ");

			CppUtils::Log::Logger::logInformation(string);
			ASSERT(string == "A, B, C");
		}),

		CppUtils::Test::UnitTest("String/cstringArrayToVectorOfStrings", [] {
			const char* cstringArray[] = {"A", "B", "C"};
			const auto strings = CppUtils::String::cstringArrayToVectorOfStrings(cstringArray, 3);

			ASSERT(strings.size() == 3);

			for (auto i = 0u; i < 3; ++i)
			{
				const auto& string = strings.at(i);
				ASSERT(std::string{cstringArray[i]} == string);
				CppUtils::Log::Logger::logInformation(string, false);
			}
			CppUtils::Log::Logger::logInformation("");
		})

	};
}
