#pragma once

#include <iostream>
#include <concepts>
#include <type_traits>

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
	template<class T, template<class...> class IncompleteType>
	inline constexpr bool isSpecializationOf = false;

	template<template<class...> class IncompleteType, class... Args>
	inline constexpr bool isSpecializationOf<IncompleteType<Args...>, IncompleteType> = true;

	template<class T, template<class...> class IncompleteType>
	concept Specializes = isSpecializationOf<T, IncompleteType>;

	template<class T>
	concept Function = std::is_function_v<T>;

	template<class T>
	inline constexpr bool isFunctionPointer = std::is_pointer_v<T> && std::is_function_v<std::remove_pointer_t<T>>;

	template<class T>
	concept FunctionPointer = isFunctionPointer<T>;

	template<class T>
	concept TriviallyConstructible = std::is_trivially_constructible_v<T>;

	template<class T>
	concept TriviallyCopyable = std::is_trivially_copyable_v<T>;
}
