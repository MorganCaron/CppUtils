#include <iostream>
#include <vector>

#include "CppUtils.hpp"

int main()
{
	const auto tests = std::vector<CppUtils::Test>{
		CppUtils::Test("Unit tests", [] {
			ASSERT(true);
		})
	};

	return CppUtils::Test::executeTests(tests);
}
