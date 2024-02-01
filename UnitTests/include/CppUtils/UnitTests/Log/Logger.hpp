#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::Logger
{
	auto _ = TestSuite{"Log/Logger", [](auto& suite) {
		using Logger = CppUtils::Logger<"CppUtils">;

		suite.addTest("In console", [&] {
			Logger::print("Message\n");

			Logger::print<"info">("Information message");
			Logger::print<"important">("Important message");
			Logger::print<"success">("Success message");
			Logger::print<"debug">("Debug message");
			Logger::print<"detail">("Detail message");
			Logger::print<"warning">("Warning message");
			Logger::print<"error">("Error message");
			suite.expect(true);
		});

		suite.addTest("In file", [&] {
			CppUtils::FileSystem::TemporaryDirectory{[&suite](const auto& directory) -> void {
				const auto logPath = directory / "test.tmp";
				auto logFile = std::ofstream{logPath, std::ios::app};

				Logger::print(logFile, "Message");

				Logger::print<"info">(logFile, "Information message");
				Logger::print<"important">(logFile, "Important message");
				Logger::print<"success">(logFile, "Success message");
				Logger::print<"debug">(logFile, "Debug message");
				Logger::print<"detail">(logFile, "Detail message");
				Logger::print<"warning">(logFile, "Warning message");
				Logger::print<"error">(logFile, "Error message");
				suite.expect(true);
			}};
		});
	}};
}
