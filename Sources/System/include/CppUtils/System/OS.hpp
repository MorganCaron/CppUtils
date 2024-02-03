#pragma once

#if defined(_WIN32) or defined(_WIN64)
#	define OS_WINDOWS
#elif defined(__APPLE__)
#	define OS_MACOS
#elif defined(__linux__) or defined(__unix__) or defined(__unix)
#	define OS_LINUX
#else
#	error unsupported platform
#endif
