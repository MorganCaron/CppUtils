#pragma once

#include <tuple>
#include <variant>

#include <CppUtils/Type/Concept.hpp>

namespace CppUtils::Type
{
	template<std::size_t I, class... Types> requires (I < sizeof...(Types))
	using NthType = std::tuple_element_t<I, std::tuple<Types...>>;



	template<class T, class... Types>
	struct Unique: std::type_identity<T>
	{};

	template<template<class...> class Container, class... Types, class Candidate, class... NextCandidates>
	struct Unique<Container<Types...>, Candidate, NextCandidates...>:
		std::conditional_t<(std::is_same_v<Candidate, Types> || ...),
			Unique<Container<Types...>, NextCandidates...>,
			Unique<Container<Types..., Candidate>, NextCandidates...>>
	{};

	template<template<class...> class Container, class... Types>
	using UniqueType = typename Unique<Container<>, Types...>::type;

	template<class... Types>
	using UniqueVariant = UniqueType<std::variant, Types...>;

	template<class... Types>
	using UniqueTuple = UniqueType<std::tuple, Types...>;



	template<template<class> class Trait, class T, class... Types>
	struct Filter: std::type_identity<T>
	{};

	template<template<class> class Trait, template<class...> class Container, class... Types, class Candidate, class... NextCandidates>
	struct Filter<Trait, Container<Types...>, Candidate, NextCandidates...>:
		std::conditional_t<Trait<Candidate>::value,
			Filter<Trait, Container<Types..., Candidate>, NextCandidates...>,
			Filter<Trait, Container<Types...>, NextCandidates...>>
	{};

	template<template<class> class Trait, template<class...> class Container, class... Types>
	using FilterType = typename Filter<Trait, Container<>, Types...>::type;
}
