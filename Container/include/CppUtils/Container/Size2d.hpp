#pragma once

#include <concepts>

namespace CppUtils::Container
{
	template<std::unsigned_integral T = std::size_t>
	struct Size2d
	{
		Size2d(T c_width, T c_height):
			width{c_width},
			height{c_height}
		{}

		T width, height;
	};
}
