#pragma once

#include <CppUtils.hpp>

namespace UnitTests::String::Parameters
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("String/Parameters/parseParameters", [] {
			const auto argc = 8;
			const char* argv[] = {"executable", "A[aaa]", "B[", "0", "]", "C", "DDD[]", "E[e e e ]"};
			const auto parameters = CppUtils::String::Parameters::parseParameters(argc, argv);

			for (const auto& [command, value] : parameters)
				CppUtils::Logger::logInformation(command + "["s + value + "]");
			
			ASSERT(parameters.size() == 5);
			ASSERT(parameters.at("A") == "aaa");
			ASSERT(parameters.at("B") == "0");
			ASSERT(parameters.at("C") == "");
			ASSERT(parameters.at("DDD") == "");
			ASSERT(parameters.at("E") == "e e e");
		})

	};
}
