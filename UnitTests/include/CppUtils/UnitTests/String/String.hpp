#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::String
{
	TEST_GROUP("String")
	{
		using namespace std::literals;

		addTest("concatenateStringsWithDelimiter", [] {
			const auto strings = std::vector{"A"sv, "B"sv, "C"sv};
			const auto string = CppUtils::String::concatenateStringsWithDelimiter(strings, ", ");

			CppUtils::Log::Logger::logInformation(string);
			TEST_ASSERT(string == "A, B, C");
		});

		addTest("cstringArrayToVectorOfStrings", [] {
			const char* cstringArray[] = {"A", "B", "C"};
			const auto strings = CppUtils::String::cstringArrayToVectorOfStrings<std::string_view>(cstringArray, 3);

			TEST_ASSERT(strings.size() == 3);

			for (auto i = 0u; i < 3; ++i)
			{
				const auto& string = strings.at(i);
				TEST_ASSERT(std::string{cstringArray[i]} == string);
				CppUtils::Log::Logger::logInformation(string, false);
			}
			CppUtils::Log::Logger::logInformation("");
		});

		addTest("trimString", [] {
			const auto string = "\n \t Hello World!\n \t ";
			const auto leftTrimString = CppUtils::String::leftTrimString(string);
			const auto rightTrimString = CppUtils::String::rightTrimString(string);
			const auto trimString = CppUtils::String::trimString(string);
			
			CppUtils::Log::Logger::logInformation('[' + std::string{leftTrimString} + ']');
			TEST_ASSERT(leftTrimString == "Hello World!\n \t ");

			CppUtils::Log::Logger::logInformation('[' + std::string{rightTrimString} + ']');
			TEST_ASSERT(rightTrimString == "\n \t Hello World!");

			CppUtils::Log::Logger::logInformation('[' + std::string{trimString} + ']');
			TEST_ASSERT(trimString == "Hello World!");
		});

		addTest("getPrintableChar", [] {
			auto output = ""s;
			for (char c = 0; c >= 0; ++c)
				output += CppUtils::String::getPrintableChar(c) + ' ';
			CppUtils::Log::Logger::logInformation(output);
		});
	}
}
