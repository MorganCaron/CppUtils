# [CppUtils](../README.md) / Container

[![Back to Parent Page](https://img.shields.io/badge/-Back_to_Parent_Page-blue?style=for-the-badge)](../README.md)

## Vector

The ``merge({a, b, c})`` function returns the merged result of the vectors a, b and c.

<p align="center"><img src="resources/Vector.drawio.svg" alt="Vector diagram"/></p>

### Example
```cpp
#include <CppUtils.hpp>

int main()
{
	const auto vector1 = std::vector<int>{ 1, 2, 3 };
	const auto vector2 = std::vector<int>{ 4, 5, 6 };
	const auto vector3 = CppUtils::Container::Vector::merge({ vector1, vector2 });
	
	for (const auto nb : vector3)
		std::cout << nb << '\n';
	
	return 0;
}
```

---
