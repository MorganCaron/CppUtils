#pragma once

#include <iostream>

#include <CppUtils/Platform/OS.hpp>
#include <CppUtils/Platform/Windows.hpp>
#include <CppUtils/Container/Size2d.hpp>

#if defined(OS_MAC) or defined(OS_LINUX)
#	include <sys/ioctl.h>
#	include <unistd.h>
#endif

namespace CppUtils::Terminal
{
#if defined(OS_WINDOWS)
	[[nodiscard]] inline auto getTerminalHandle(std::ostream& stream) -> HANDLE
	{
		HANDLE terminalHandle = INVALID_HANDLE_VALUE;
		if (&stream == &std::cout)
			terminalHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		else if (&stream == &std::cerr)
			terminalHandle = GetStdHandle(STD_ERROR_HANDLE);
		return terminalHandle;
	}
#endif

	inline auto setConsoleOutputUTF8() -> void
	{
#if defined(OS_WINDOWS)
		SetConsoleOutputCP(CP_UTF8);
#endif
	}

	[[nodiscard]] inline auto getTerminalSize() -> Container::Size2d<>
	{
#if defined(OS_WINDOWS)
		auto consoleScreenBufferInfo = CONSOLE_SCREEN_BUFFER_INFO{};
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleScreenBufferInfo);
		return Container::Size2d<>{
			consoleScreenBufferInfo.srWindow.Right - consoleScreenBufferInfo.srWindow.Left + 1,
			consoleScreenBufferInfo.srWindow.Bottom - consoleScreenBufferInfo.srWindow.Top + 1
		};
#elif defined(OS_MAC) or defined(OS_LINUX)
		auto windowsSize = winsize{};
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowsSize);
		return Container::Size2d<>{
			windowsSize.ws_col,
			windowsSize.ws_row
		};
#endif
	}
}
