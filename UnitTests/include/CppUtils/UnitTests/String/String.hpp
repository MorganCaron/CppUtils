#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::String
{
	TEST_GROUP("String")
	{
		using namespace std::literals;
		using Logger = CppUtils::Logger<"CppUtils">;

		addTest("concatenateStringsWithDelimiter", [] {
			auto strings = std::vector{"A"sv, "B"sv, "C"sv};
			auto string = CppUtils::String::concatenateStringsWithDelimiter(strings, ", ");

			Logger::print(string);
			EXPECT(string == "A, B, C");
		});

		addTest("trimString", [] {
			auto string = "\n \t Hello World!\n \t ";
			auto leftTrimString = CppUtils::String::leftTrimString(string);
			auto rightTrimString = CppUtils::String::rightTrimString(string);
			auto trimString = CppUtils::String::trimString(string);

			Logger::print(R"("{}")", leftTrimString);
			EXPECT(leftTrimString == "Hello World!\n \t ");

			Logger::print(R"("{}")", rightTrimString);
			EXPECT(rightTrimString == "\n \t Hello World!");

			Logger::print(R"("{}")", trimString);
			EXPECT(trimString == "Hello World!");
		});

		addTest("getPrintableChar", [] {
			auto output = ""s;
			for (char c = 0; c >= 0; ++c)
				output += CppUtils::String::getPrintableChar(c) + ' ';
			Logger::print(output);
		});
	}
}
