#pragma once

#include <CppUtils/Platform/OS.hpp>

#if defined(OS_WINDOWS)
#	ifdef STATIC_LIB
#		define DLL_PUBLIC
#	else
#		ifdef BUILDING_DLL
#			define DLL_PUBLIC __declspec(dllexport)
#		else
#			define DLL_PUBLIC __declspec(dllimport)
#		endif
#	endif
#else
#	define DLL_PUBLIC
#endif
