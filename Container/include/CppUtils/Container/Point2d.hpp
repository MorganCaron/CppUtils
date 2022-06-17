#pragma once

namespace CppUtils::Container
{
	template<class T>
	struct Point2d
	{
		Point2d(T c_x, T c_y):
			x{c_x},
			y{c_y}
		{}
		
		T x, y;
	};
}
