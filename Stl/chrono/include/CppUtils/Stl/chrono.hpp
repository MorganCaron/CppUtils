#pragma once

#include <type_traits>

namespace CppUtils::Stl::chrono
{
	// https://en.cppreference.com/w/cpp/chrono/is_clock
	template<class>
	struct is_clock: std::false_type
	{};

	template<class T>
	requires requires {
		typename T::rep;
		typename T::period;
		typename T::duration;
		typename T::time_point;
		T::is_steady;
		T::now();
	}
	struct is_clock<T>: std::true_type
	{};

	// Not yet supported on Clang
	template<class T>
	constexpr auto is_clock_v = is_clock<T>::value;
}
