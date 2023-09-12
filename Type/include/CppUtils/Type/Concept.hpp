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

	template<std::size_t N, class... Types>
	concept AtLeastNType = (sizeof...(Types) >= N);

	template<class T, template<class...> class IncompleteType>
	constexpr bool isSpecializationOf = false;

	template<template<class...> class IncompleteType, class... Args>
	constexpr bool isSpecializationOf<IncompleteType<Args...>, IncompleteType> = true;

	template<class T, template<class...> class IncompleteType>
	concept Specializes = isSpecializationOf<T, IncompleteType>;
}
