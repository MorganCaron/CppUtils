#pragma once

#include <iostream>
#include <concepts>

namespace CppUtils::Type::Concept
{
	template<typename T>
	concept isPrintable = requires(T rhs) {
		{ std::cout << rhs } -> std::same_as<std::ostream&>;
	};

	template<typename Type, typename... Types>
	concept isPresent = (std::is_same_v<Type, Types> || ...);
}
