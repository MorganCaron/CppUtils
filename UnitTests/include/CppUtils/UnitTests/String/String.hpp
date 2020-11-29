#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::String
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest{"String/concatenateStringsWithDelimiter", [] {
			using namespace std::literals;
			
			const auto strings = std::vector{"A"s, "B"s, "C"s};
			const auto string = CppUtils::String::concatenateStringsWithDelimiter(strings, ", ");

			CppUtils::Log::Logger::logInformation(string);
			ASSERT(string == "A, B, C");
		}},

		CppUtils::Test::UnitTest{"String/cstringArrayToVectorOfStrings", [] {
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
		}},

		CppUtils::Test::UnitTest{"String/trimString", [] {
			const auto string = "\n \t Hello World!\n \t ";
			const auto leftTrimString = CppUtils::String::leftTrimString(string);
			const auto rightTrimString = CppUtils::String::rightTrimString(string);
			const auto trimString = CppUtils::String::trimString(string);
			
			CppUtils::Log::Logger::logInformation('[' + std::string{leftTrimString} + ']');
			ASSERT(leftTrimString == "Hello World!\n \t ");

			CppUtils::Log::Logger::logInformation('[' + std::string{rightTrimString} + ']');
			ASSERT(rightTrimString == "\n \t Hello World!");

			CppUtils::Log::Logger::logInformation('[' + std::string{trimString} + ']');
			ASSERT(trimString == "Hello World!");
		}}

	};
}
