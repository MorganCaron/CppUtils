#pragma once

#include <iostream>

#include <CppUtils/Container/Size2d.hpp>
#include <CppUtils/System/Windows.hpp>

#if !defined(OS_WINDOWS)
#	include <cstdint>
#endif

namespace CppUtils::Terminal
{
#if defined(OS_WINDOWS)
	// HANDLE is defined as a void pointer see https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types#handle
	using Handle = void*;
#else
	using Handle = std::intptr_t;
#endif

#if defined(OS_WINDOWS)
	// INVALID_HANDLE_VALUE is defined as https://github.com/tpn/winsdk-10/blob/9b69fd26ac0c7d0b83d378dba01080e93349c2ed/Include/10.0.10240.0/um/handleapi.h#L43
	// it can't be constexpr
	inline const auto InvalidHandle = reinterpret_cast<Handle>(std::uintptr_t{-1});
#else
	inline constexpr auto InvalidHandle = Handle{-1};
#endif

	auto setConsoleOutputUTF8() -> void;
	[[nodiscard]] auto getTerminalHandle(std::FILE* file) -> Handle;
	[[nodiscard]] auto getTerminalHandle(std::ostream& stream) -> Handle;
	[[nodiscard]] auto getTerminalSize() -> Container::Size2d<>;
}
