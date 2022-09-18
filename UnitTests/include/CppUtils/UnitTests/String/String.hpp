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

			CppUtils::Log::Logger{std::cout} << string << '\n';
			TEST_ASSERT(string == "A, B, C");
		});

		addTest("cStringArrayToVectorOfStrings", [] {
			const char* cstringArray[] = {"A", "B", "C"};
			const auto strings = CppUtils::String::cStringArrayToVectorOfStrings<std::string_view>(cstringArray, 3);

			TEST_ASSERT(strings.size() == 3);

			auto logger = CppUtils::Log::Logger{std::cout};
			for (auto i = 0u; i < 3; ++i)
			{
				const auto& string = strings.at(i);
				TEST_ASSERT(std::string{cstringArray[i]} == string);
				logger << string;
			}
			logger << '\n';
		});

		addTest("trimString", [] {
			const auto string = "\n \t Hello World!\n \t ";
			const auto leftTrimString = CppUtils::String::leftTrimString(string);
			const auto rightTrimString = CppUtils::String::rightTrimString(string);
			const auto trimString = CppUtils::String::trimString(string);
			auto logger = CppUtils::Log::Logger{std::cout};

			logger << '"' << leftTrimString << "\"\n";
			TEST_ASSERT(leftTrimString == "Hello World!\n \t ");

			logger << '"' << rightTrimString << "\"\n";
			TEST_ASSERT(rightTrimString == "\n \t Hello World!");

			logger << '"' << trimString << "\"\n";
			TEST_ASSERT(trimString == "Hello World!");
		});

		addTest("getPrintableChar", [] {
			auto output = ""s;
			for (char c = 0; c >= 0; ++c)
				output += CppUtils::String::getPrintableChar(c) + ' ';
			CppUtils::Log::Logger{std::cout} << output << '\n';
		});
	}
}
