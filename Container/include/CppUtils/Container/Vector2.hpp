#pragma once

namespace CppUtils::Container
{
	template<class T>
	struct Vector2
	{
		Vector2(T c_x, T c_y):
			x{c_x},
			y{c_y}
		{}

		T x, y;
	};
}
