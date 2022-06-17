#pragma once

#include <chrono>
#include <iostream>
#include <concepts>
#include <string_view>

namespace CppUtils::Type::Concept
{
	template<class T>
	concept Printable = requires(T value) {
		{ std::cout << value } -> std::same_as<std::ostream&>;
	};

	template<class Type, class... Types>
	concept Present = (std::same_as<Type, Types> || ...);
	
	template<class T>
	concept Dereferenceable = requires(T value) {
		*value;
	};

	template<std::size_t N, class... Types>
	concept AtLeastNType = (sizeof...(Types) >= N);

	template<class T>
	concept String = std::is_convertible_v<T, std::string_view>;

	template<class T>
	concept Clock = std::chrono::is_clock_v<T>;
}
