#pragma once

#include <tuple>
#include <iostream>
#include <concepts>

namespace CppUtils::Type::Traits
{
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

	template<std::size_t N, typename... Types>
	concept containAtLeastNType = (std::tuple_size<std::tuple<Types...>>{} >= N);
}
