#pragma once

#include <CppUtils.hpp>

namespace UnitTests::Terminal::Parameters
{
	struct ProgramSettings
	{
		bool verbose = false;
		std::string parameter;
	};

	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("Terminal/Parameters/parseParameters", [] {
			const auto argc = 8;
			const char* argv[] = {"executable", "A[aaa]", "B[", "0", "]", "C", "DDD[]", "E[e e e ]"};
			const auto parameters = CppUtils::Terminal::Parameters::parseParameters(argc, argv);

			for (const auto& [command, value] : parameters)
				CppUtils::Logger::logInformation(command + "["s + value + "]");
			
			ASSERT(parameters.size() == 5);
			ASSERT(parameters.at("A") == "aaa");
			ASSERT(parameters.at("B") == "0");
			ASSERT(parameters.at("C") == "");
			ASSERT(parameters.at("DDD") == "");
			ASSERT(parameters.at("E") == "e e e");
		}),

		CppUtils::Test::UnitTest("Terminal/Parameters/executeCommands", [] {
			const auto argc = 4;
			const char* argv[] = {"executable", "info", "verbose", "parameter[value]"};
			auto settings = ProgramSettings{};
			const auto abort = CppUtils::Terminal::Parameters::executeCommands(argc, argv, settings, {
				{
					"info",
					[]([[maybe_unused]] auto& settings, [[maybe_unused]] auto value) -> bool {
						CppUtils::Logger::logInformation("info");
						return false;
					}
				},
				{
					"verbose",
					[]([[maybe_unused]] auto& settings, [[maybe_unused]] auto value) -> bool {
						settings.verbose = true;
						return false;
					}
				},
				{
					"parameter",
					[]([[maybe_unused]] auto& settings, [[maybe_unused]] auto value) -> bool {
						settings.parameter = value;
						return false;
					}
				}
			});

			ASSERT(abort == false);

			CppUtils::Logger::logInformation("settings.verbose = "s + std::to_string(settings.verbose));
			CppUtils::Logger::logInformation("settings.parameter = "s + settings.parameter);
			
			ASSERT(settings.verbose == true);
			ASSERT(settings.parameter == "value");
		})

	};
}
