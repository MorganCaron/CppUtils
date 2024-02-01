#pragma once

#include <chrono>

#include <CppUtils/Stl/chrono.hpp>
#include <CppUtils/Type/Concept.hpp>

namespace CppUtils::Chrono::Concept
{
	template<class T>
	concept Duration = Type::Concept::Specializes<std::remove_cvref_t<T>, std::chrono::duration>;

	template<class T>
	concept Clock = Stl::chrono::is_clock_v<T>;
}
