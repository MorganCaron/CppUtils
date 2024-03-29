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
			EXPECT(parameters.size() == 5);
			EXPECT(parameters.at("A") == "aaa");
			EXPECT(parameters.at("B") == "0");
			EXPECT(parameters.at("C") == "");
			EXPECT(parameters.at("DDD") == "");
			EXPECT(parameters.at("E") == "e e e");
		});

		addTest("executeCommands", [] {
			const auto argc = 4;
			const char* argv[] = {"executable", "info", "verbose", "parameter[value]"};
			auto settings = ProgramSettings{};
			const auto parametersLexer = CppUtils::Parameters::ParametersLexer{};
			const auto abort = parametersLexer.executeCommands(argc, argv, {{"info", []([[maybe_unused]] auto value) -> bool {
				CppUtils::Log::Logger::logInformation("info");
				return false;
			}},
																			   {"verbose", [&settings]([[maybe_unused]] auto value) -> bool {
				settings.verbose = true;
				return false;
			}},
																			   {"parameter", [&settings]([[maybe_unused]] auto value) -> bool {
				settings.parameter = value;
				return false;
			}}});
			EXPECT(abort == false);
			CppUtils::Log::Logger::logInformation("settings.verbose = "s + std::to_string(settings.verbose));
			CppUtils::Log::Logger::logInformation("settings.parameter = "s + settings.parameter);
			EXPECT(settings.verbose == true);
			EXPECT(settings.parameter == "value");
		});
	}
}
