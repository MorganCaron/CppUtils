#pragma once

#include <typeindex>

namespace CppUtils::Type
{
	using TypeIndex = std::size_t;

	template<typename T>
	struct Type
	{
		using Wrapped = T;
		static const TypeIndex index;
	};

	template<typename T>
	const TypeIndex Type<T>::index = std::hash<std::type_index>()(std::type_index(typeid(T)));
}
