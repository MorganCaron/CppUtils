# [CppUtils](../README.md) / Hash

[![Back to Parent Page](https://img.shields.io/badge/-Back_to_Parent_Page-blue?style=for-the-badge)](../README.md)

The ``hash(string)`` function allows to hash a string during compilation.
The hash allows to obtain an identifier from the string filled in.

<p align="center"><img src="resources/Hash.drawio.svg" alt="Hash diagram"/></p>

### Example
```cpp
#include <CppUtils.hpp>

int main()
{
	const auto id = CppUtils::Hash::hash("Text");

	std::cout << id << std::endl;

	return 0;
}
```

---
