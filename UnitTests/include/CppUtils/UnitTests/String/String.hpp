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
