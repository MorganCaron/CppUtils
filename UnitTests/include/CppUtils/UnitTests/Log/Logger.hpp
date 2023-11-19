#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Logger
{
	TEST_GROUP("Log/Logger")
	{
		using Logger = CppUtils::Logger<"CppUtils">;

		addTest("In console", [] {
			Logger::print("Message");

			Logger::print<"info">("Information message");
			Logger::print<"important">("Important message");
			Logger::print<"success">("Success message");
			Logger::print<"debug">("Debug message");
			Logger::print<"detail">("Detail message");
			Logger::print<"warning">("Warning message");
			Logger::print<"error">("Error message");
			EXPECT(true);
		});

		addTest("In file", [] {
			const auto logPath = std::filesystem::path{"test.tmp"};
			auto logFile = std::ofstream{logPath, std::ios::app};

			Logger::print(logFile, "Message");

			Logger::print<"info">(logFile, "Information message");
			Logger::print<"important">(logFile, "Important message");
			Logger::print<"success">(logFile, "Success message");
			Logger::print<"debug">(logFile, "Debug message");
			Logger::print<"detail">(logFile, "Detail message");
			Logger::print<"warning">(logFile, "Warning message");
			Logger::print<"error">(logFile, "Error message");

			CppUtils::FileSystem::File::deleteFile(logPath);
			EXPECT(true);
		});
	}
}
