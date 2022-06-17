#pragma once

#include <memory>
#include <functional>

namespace CppUtils::Memory
{
	template<class T>
	using UniquePtrWithDestructor = std::unique_ptr<T, std::function<void(T*)>>;
}
