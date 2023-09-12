#pragma once

#include <chrono>

#include <CppUtils/Type/Concept.hpp>
#include <CppUtils/Stl/chrono.hpp>

namespace CppUtils::Chrono::Concept
{
	template<class T>
	concept Duration = Type::Concept::Specializes<T, std::chrono::duration>;

	template<class T>
	concept Clock = std::chrono::is_clock_v<T>;
}
