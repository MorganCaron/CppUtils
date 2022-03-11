#pragma once

#if defined(_MSC_VER)
#	define COMPILER_MSVC
#elif defined(__GNUC__)
#	define COMPILER_GCC
#elif defined(__clang__)
#	define COMPILER_CLANG
#else
#	error unsupported compiler
#endif

#if defined(COMPILER_MSVC)
#	pragma warning(disable: 4251)
#endif
