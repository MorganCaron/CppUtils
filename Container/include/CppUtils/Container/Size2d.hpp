#pragma once

namespace CppUtils::Container
{
	template<typename T>
	struct Size2d
	{
		Size2d(T c_width, T c_height):
			width{c_width},
			height{c_height}
		{}
		
		T width, height;
	};
}
