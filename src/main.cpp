#include <iostream>
#include <vector>

#include "CppUtils.hpp"

int executeTests(const std::vector<CppUtils::Test>& tests)
{
	for (const auto& test : tests)
	{
		if (!test.pass())
		{
			std::cout << "The tests failed" << std::endl;
			return EXIT_FAILURE;
		}
	}
	
	std::cout << "The tests passed" << std::endl;
	return EXIT_SUCCESS;
}

int main()
{
	const auto tests = std::vector<CppUtils::Test>{
		CppUtils::Test("Unit tests", [] {
			ASSERT(false);
		})
	};

	return executeTests(tests);
}
