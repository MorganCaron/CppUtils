#pragma once

#include <CppUtils/Container/Vector2.hpp>

namespace CppUtils::Container
{
	template<class T>
	struct Vector3: public Vector2<T>
	{
		Vector3(T c_x, T c_y, T c_z):
			Vector2<T>{c_x, c_y},
			z{c_z}
		{}

		T z;
	};
}
