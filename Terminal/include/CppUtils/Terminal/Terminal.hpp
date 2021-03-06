#pragma once

#include <iostream>

#include <CppUtils/Platform/OS.hpp>

#if defined(OS_WINDOWS)
#	include <Windows.h>
#endif

namespace CppUtils::Terminal
{
#if defined(OS_WINDOWS)
	[[nodiscard]] inline HANDLE getTerminalHandle(std::ostream& stream)
	{
		HANDLE terminalHandle = INVALID_HANDLE_VALUE;
		if (&stream == &std::cout)
			terminalHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		else if (&stream == &std::cerr)
			terminalHandle = GetStdHandle(STD_ERROR_HANDLE);
		return terminalHandle;
	}
#endif

	inline void setConsoleOutputUTF8()
	{
#if defined(OS_WINDOWS)
		SetConsoleOutputCP(CP_UTF8);
#endif
	}
}
