#pragma once

#include <CppUtils/Platform/OS.hpp>
#include <CppUtils/Platform/Windows.hpp>

#if defined(OS_LINUX) || defined(OS_MACOS)
#	include <dlfcn.h>
#endif

namespace CppUtils::Platform
{
#if defined(OS_WINDOWS)
	using Library = HMODULE;
#elif defined(OS_LINUX) || defined(OS_MACOS)
	using Library = void*;
#else
#	error unsupported platform
#endif
}
