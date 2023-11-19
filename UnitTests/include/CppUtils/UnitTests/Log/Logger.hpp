#pragma once

#include <CppUtils.hpp>

namespace CppUtils
{
	template<>
	template<>
	auto Logger<"LoggerName">::format<"info">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stdout, Terminal::TextColor::TextColorEnum::Default},
			std::format("INFO: {}\n", message)
		};
	}

	template<>
	template<>
	auto Logger<"LoggerName">::format<"important">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stdout, Terminal::TextColor::TextColorEnum::Cyan},
			std::format("{}\n", message)
		};
	}

	template<>
	template<>
	auto Logger<"LoggerName">::format<"success">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stdout, Terminal::TextColor::TextColorEnum::Green},
			std::format("{}\n", message)
		};
	}

	template<>
	template<>
	auto Logger<"LoggerName">::format<"debug">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stdout, Terminal::TextColor::TextColorEnum::Magenta},
			std::format("DEBUG: {}\n", message)
		};
	}

	template<>
	template<>
	auto Logger<"LoggerName">::format<"detail">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stdout, Terminal::TextColor::TextColorEnum::Blue},
			std::format("{}\n", message)
		};
	}

	template<>
	template<>
	auto Logger<"LoggerName">::format<"warning">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stdout, Terminal::TextColor::TextColorEnum::Yellow},
			std::format("WARNING: {}\n", message)
		};
	}

	template<>
	template<>
	auto Logger<"LoggerName">::format<"error">(std::string_view message) -> Formatter
	{
		return {
			Terminal::TextModifier{stderr, Terminal::TextColor::TextColorEnum::Red},
			std::format("ERROR: {}\n", message)
		};
	}
}

namespace CppUtils::UnitTests::Logger
{
	TEST_GROUP("Log/Logger")
	{
		using TestLogger = CppUtils::Logger<"LoggerName">;

		addTest("In console", [] {
			TestLogger::print("Message");

			TestLogger::print<"info">("Information message");
			TestLogger::print<"important">("Important message");
			TestLogger::print<"success">("Success message");
			TestLogger::print<"debug">("Debug message");
			TestLogger::print<"detail">("Detail message");
			TestLogger::print<"warning">("Warning message");
			TestLogger::print<"error">("Error message");
			EXPECT(true);
		});

		addTest("In file", [] {
			const auto logPath = std::filesystem::path{"test.tmp"};
			auto logFile = std::ofstream{logPath, std::ios::app};

			TestLogger::print(logFile, "Message");

			TestLogger::print<"info">(logFile, "Information message");
			TestLogger::print<"important">(logFile, "Important message");
			TestLogger::print<"success">(logFile, "Success message");
			TestLogger::print<"debug">(logFile, "Debug message");
			TestLogger::print<"detail">(logFile, "Detail message");
			TestLogger::print<"warning">(logFile, "Warning message");
			TestLogger::print<"error">(logFile, "Error message");

			CppUtils::FileSystem::File::deleteFile(logPath);
			EXPECT(true);
		});
	}
}
