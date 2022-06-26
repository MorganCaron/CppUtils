#pragma once

#include <string>

#include <CppUtils/Terminal/Terminal.hpp>

namespace CppUtils::UnitTest
{
	struct TestSettings final
	{
		Container::Size2d<> terminalSize = Terminal::getTerminalSize();
		bool verbose = true;
		bool detail = true;
		bool failFast = false;
		bool chrono = false;
		bool abort = false;
		std::string filter;
	};
}
