#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Parameters
{
	using namespace std::literals;
	
	struct ProgramSettings
	{
		bool verbose = false;
		std::string parameter;
	};

	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("Language/Parameters/parseParameters", [] {
			const auto argc = 8;
			const char* argv[] = {"executable", "A[aaa]", "B[", "0", "]", "C", "DDD[]", "E[e e e ]"};
			const auto parametersLexer = CppUtils::Language::Parameters::ParametersLexer{};
			const auto parameters = parametersLexer.parseParameters(argc, argv);

			for (const auto& [command, value] : parameters)
				CppUtils::Log::Logger::logInformation(command + "["s + value + "]");
			
			ASSERT(parameters.size() == 5);
			ASSERT(parameters.at("A") == "aaa");
			ASSERT(parameters.at("B") == "0");
			ASSERT(parameters.at("C") == "");
			ASSERT(parameters.at("DDD") == "");
			ASSERT(parameters.at("E") == "e e e");
		}),

		CppUtils::Test::UnitTest("Language/Parameters/executeCommands", [] {
			const auto argc = 4;
			const char* argv[] = {"executable", "info", "verbose", "parameter[value]"};
			auto settings = ProgramSettings{};
			const auto parametersLexer = CppUtils::Language::Parameters::ParametersLexer{};
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

			ASSERT(abort == false);

			CppUtils::Log::Logger::logInformation("settings.verbose = "s + std::to_string(settings.verbose));
			CppUtils::Log::Logger::logInformation("settings.parameter = "s + settings.parameter);
			
			ASSERT(settings.verbose == true);
			ASSERT(settings.parameter == "value");
		})

	};
}
