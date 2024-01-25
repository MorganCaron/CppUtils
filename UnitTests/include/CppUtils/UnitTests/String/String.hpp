#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::String
{
	auto _ = TestSuite{"String", [](auto& suite) {
		using namespace std::literals;
		using Logger = CppUtils::Logger<"CppUtils">;

		suite.addTest("concatenateStringsWithDelimiter", [&] {
			auto strings = std::vector{"A"sv, "B"sv, "C"sv};
			auto string = CppUtils::String::concatenateStringsWithDelimiter(strings, ", ");

			Logger::print("{}\n", string);
			suite.expectEqual(string, "A, B, C");
		});

		suite.addTest("trimString", [&] {
			auto string = "\n \t Hello World!\n \t ";
			auto leftTrimString = CppUtils::String::leftTrimString(string);
			auto rightTrimString = CppUtils::String::rightTrimString(string);
			auto trimString = CppUtils::String::trimString(string);

			Logger::print("\"{}\"\n", leftTrimString);
			suite.expectEqual(leftTrimString, "Hello World!\n \t ");

			Logger::print("\"{}\"\n", rightTrimString);
			suite.expectEqual(rightTrimString, "\n \t Hello World!");

			Logger::print("\"{}\"\n", trimString);
			suite.expectEqual(trimString, "Hello World!");
		});

		suite.addTest("getPrintableChar", [&] {
			auto output = ""s;
			for (char c = 0; c >= 0; ++c)
				output += CppUtils::String::getPrintableChar(c) + ' ';
			Logger::print("{}\n", output);
		});
	}};
}
