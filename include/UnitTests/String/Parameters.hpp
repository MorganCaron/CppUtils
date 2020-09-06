#pragma once

#include <CppUtils.hpp>

namespace UnitTests::String::Parameters
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("String/Parameters/parseParameters", [] {
			const auto argc = 6;
			const char* argv[] = {"executable", "A[aaa]", "B[", "0", "]", "C"};
			const auto parameters = CppUtils::String::Parameters::parseParameters(argc, argv);

			ASSERT(parameters.size() == 3);
			ASSERT(parameters.at("A") == "aaa");
			ASSERT(parameters.at("B") == "0");
			ASSERT(parameters.at("C") == "");
		})

	};
}
