#pragma once

#if defined(_WIN32) || defined(_WIN64)
#	define OS_WINDOWS
#elif defined(__APPLE__)
#	define OS_MACOS
#elif defined(__unix__) || defined(__unix)
#	define OS_LINUX
#else
#	error unsupported platform
#endif
