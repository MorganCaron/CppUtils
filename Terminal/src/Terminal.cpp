#include <CppUtils/Terminal/Terminal.hpp>

#include <iostream>

#include <CppUtils/Platform/Windows.hpp>
#include <CppUtils/Container/Size2d.hpp>

#if defined(OS_MAC) or defined(OS_LINUX)
#	include <sys/ioctl.h>
#	include <unistd.h>
#endif

namespace CppUtils::Terminal
{
	auto setConsoleOutputUTF8() -> void
	{
#if defined(OS_WINDOWS)
		SetConsoleOutputCP(CP_UTF8);
#endif
	}

#if defined(OS_WINDOWS)
	[[nodiscard]] auto getTerminalHandle(std::FILE* file) -> Handle
	{
		auto terminalHandle = InvalidHandle;
		if (file == stdout)
			terminalHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		else if (file == stderr)
			terminalHandle = GetStdHandle(STD_ERROR_HANDLE);
		return terminalHandle;
	}

	[[nodiscard]] auto getTerminalHandle(std::ostream& stream) -> Handle
	{
		auto terminalHandle = InvalidHandle;
		if (&stream == &std::cout)
			terminalHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		else if (&stream == &std::cerr)
			terminalHandle = GetStdHandle(STD_ERROR_HANDLE);
		return terminalHandle;
	}
#else
	[[nodiscard]] auto getTerminalHandle([[maybe_unused]] std::FILE* file) -> Handle
	{
		return InvalidHandle;
	}

	[[nodiscard]] auto getTerminalHandle([[maybe_unused]] std::ostream& stream) -> Handle
	{
		return InvalidHandle;
	}
#endif

#if defined(OS_WINDOWS)
	[[nodiscard]] auto getTerminalSize() -> Container::Size2d<>
	{
		auto consoleScreenBufferInfo = CONSOLE_SCREEN_BUFFER_INFO{};
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleScreenBufferInfo);
		return Container::Size2d<>{
			static_cast<std::size_t>(consoleScreenBufferInfo.srWindow.Right - consoleScreenBufferInfo.srWindow.Left + 1),
			static_cast<std::size_t>(consoleScreenBufferInfo.srWindow.Bottom - consoleScreenBufferInfo.srWindow.Top + 1)
		};
	}
#elif defined(OS_MAC) or defined(OS_LINUX)
	[[nodiscard]] auto getTerminalSize() -> Container::Size2d<>
	{
		auto windowsSize = winsize{};
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowsSize);
		return Container::Size2d<>{
			windowsSize.ws_col,
			windowsSize.ws_row
		};
	}
#endif
}
