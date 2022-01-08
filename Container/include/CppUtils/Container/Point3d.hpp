#pragma once

#include <CppUtils/Container/Point2d.hpp>

namespace CppUtils::Container
{
	template<typename T>
	struct Point3d: public Point2d<T>
	{
		Point3d(T c_x, T c_y, T c_z):
			Point2d<T>{c_x, c_y},
			z{c_z}
		{}

		T z;
	};
}
