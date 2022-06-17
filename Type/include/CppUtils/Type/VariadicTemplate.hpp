#pragma once

#include <CppUtils/Type/Concept.hpp>

namespace CppUtils::Type::VariadicTemplate
{
	template<std::size_t i, class... Types>
	requires Concept::AtLeastNType<i, Types...>
	using NthType = typename std::tuple_element<i, std::tuple<Types...>>::type;
}
