#pragma once

#include <string>

namespace CppUtils::UnitTest
{
	struct TestSettings final
	{
		bool verbose = true;
		bool detail = true;
		bool failFast = false;
		bool chrono = false;
		bool abort = false;
		std::string filter;
	};
}
