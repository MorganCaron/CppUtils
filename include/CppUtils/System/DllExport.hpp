#pragma once

#include <CppUtils/System/Compiler.hpp>
#include <CppUtils/System/OS.hpp>

#if !(defined(STATIC_LIB) or defined(SHARED_LIB))
#	error "Either STATIC_LIB or SHARED_LIB must be defined"
#endif

#if defined(STATIC_LIB)
#	define DLL_PUBLIC
#elif defined(SHARED_LIB)
#	if defined(OS_WINDOWS)
#		if defined(DLL_EXPORT)
#			define DLL_PUBLIC __declspec(dllexport)
#		else
#			define DLL_PUBLIC
#		endif
#	elif defined(OS_LINUX) or defined(OS_MACOS)
#		define DLL_PUBLIC [[gnu::visibility("default")]]
#	endif
#endif
