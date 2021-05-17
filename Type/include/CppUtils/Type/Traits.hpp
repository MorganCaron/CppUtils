#pragma once

#include <iostream>
#include <concepts>

namespace CppUtils::Type::Traits
{
	template<typename T>
	concept isAddress = std::is_integral_v<T> && std::is_unsigned_v<T> && (sizeof(T) <= sizeof(void*));

	template<typename T>
	concept isPrintable = requires(T rhs) {
		{ std::cout << rhs } -> std::same_as<std::ostream&>;
	};

	template<typename Type, typename... Types>
	concept isPresent = (std::same_as<Type, Types> || ...);
	
	template<typename T>
	concept isDereferenceable = requires(T value) {
		*value;
	};
}
