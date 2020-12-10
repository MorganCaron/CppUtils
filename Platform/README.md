# [CppUtils](../README.md) / Platform

[![Back to Parent Page](https://img.shields.io/badge/-Back_to_Parent_Page-blue?style=for-the-badge)](../README.md)

## OS

The ``OS_WINDOWS``, ``OS_MACOS`` and ``OS_LINUX`` macros let you know on which platform the project is compiled.
Each macro is declared in case the project is compiled in the associated OS. The others are not declared.

### Example
```cpp
#include <CppUtils.hpp>

int main()
{
	#if defined(OS_WINDOWS)
	std::cout << "Windows" << std::endl;
	#elif defined(OS_MACOS)
	std::cout << "MacOS" << std::endl;
	#elif defined(OS_LINUX)
	std::cout << "LINUX" << std::endl;
	#endif
}
```

---
