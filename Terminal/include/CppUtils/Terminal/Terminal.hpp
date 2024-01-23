#pragma once

#include <iostream>

#include <CppUtils/Platform/Windows.hpp>
#include <CppUtils/Container/Size2d.hpp>

#if !defined(OS_WINDOWS)
#	include <cstdint>
#endif

namespace CppUtils::Terminal
{
#if defined(OS_WINDOWS)
	using Handle = HANDLE;
#else
	using Handle = std::intptr_t;
#endif

#if defined(OS_WINDOWS)
	constexpr auto InvalidHandle = INVALID_HANDLE_VALUE;
#else
	constexpr auto InvalidHandle = static_cast<Handle>(-1);
#endif

	auto setConsoleOutputUTF8() -> void;
	[[nodiscard]] auto getTerminalHandle(std::FILE* file) -> Handle;
	[[nodiscard]] auto getTerminalHandle(std::ostream& stream) -> Handle;
	[[nodiscard]] auto getTerminalSize() -> Container::Size2d<>;
}
