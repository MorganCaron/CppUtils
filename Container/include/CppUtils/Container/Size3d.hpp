#pragma once

#include <CppUtils/Container/Size2d.hpp>

namespace CppUtils::Container
{
	template<class T>
	struct Size3d: public Size2d<T>
	{
		Size3d(T c_width, T c_height, T c_depth):
			Size2d<T>{c_width, c_height},
			depth{c_depth}
		{}

		T depth;
	};
}
