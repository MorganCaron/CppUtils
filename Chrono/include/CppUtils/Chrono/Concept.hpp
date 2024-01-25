#pragma once

#include <chrono>

#include <CppUtils/Stl/chrono.hpp>
#include <CppUtils/Type/Concept.hpp>

namespace CppUtils::Chrono::Concept
{
	template<class T>
	concept Duration = Type::Concept::Specializes<T, std::chrono::duration>;

	template<class T>
	concept Clock = Stl::chrono::is_clock_v<T>;
}
