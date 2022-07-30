#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Parameters::ParametersLexer
{
	struct ProgramSettings
	{
		bool verbose = false;
		std::string parameter;
	};

	TEST_GROUP("Parameters/ParametersLexer")
	{
		using namespace std::literals;

		addTest("parseParameters", [] {
			const auto argc = 8;
			const char* argv[] = {"executable", "A[aaa]", "B[", "0", "]", "C", "DDD[]", "E[e e e ]"};
			const auto parametersLexer = CppUtils::Parameters::ParametersLexer{};
			const auto parameters = parametersLexer.parseParameters(argc, argv);
			for (const auto& [command, value] : parameters)
				CppUtils::Log::Logger::logInformation(command + "["s + value + "]");	
			TEST_ASSERT(parameters.size() == 5);
			TEST_ASSERT(parameters.at("A") == "aaa");
			TEST_ASSERT(parameters.at("B") == "0");
			TEST_ASSERT(parameters.at("C") == "");
			TEST_ASSERT(parameters.at("DDD") == "");
			TEST_ASSERT(parameters.at("E") == "e e e");
		});

		addTest("executeCommands", [] {
			const auto argc = 4;
			const char* argv[] = {"executable", "info", "verbose", "parameter[value]"};
			auto settings = ProgramSettings{};
			const auto parametersLexer = CppUtils::Parameters::ParametersLexer{};
			const auto abort = parametersLexer.executeCommands(argc, argv, {
				{
					"info",
					[]([[maybe_unused]] auto value) -> bool {
						CppUtils::Log::Logger::logInformation("info");
						return false;
					}
				},
				{
					"verbose",
					[&settings]([[maybe_unused]] auto value) -> bool {
						settings.verbose = true;
						return false;
					}
				},
				{
					"parameter",
					[&settings]([[maybe_unused]] auto value) -> bool {
						settings.parameter = value;
						return false;
					}
				}
			});
			TEST_ASSERT(abort == false);
			CppUtils::Log::Logger::logInformation("settings.verbose = "s + std::to_string(settings.verbose));
			CppUtils::Log::Logger::logInformation("settings.parameter = "s + settings.parameter);
			TEST_ASSERT(settings.verbose == true);
			TEST_ASSERT(settings.parameter == "value");
		});
	}
}
