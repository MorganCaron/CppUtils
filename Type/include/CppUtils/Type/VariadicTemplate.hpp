#pragma once

#include <CppUtils/Type/Traits.hpp>

namespace CppUtils::VariadicTemplate
{
	template<std::size_t i, typename...Types>
	requires Type::Traits::containAtLeastNType<1, Types...>
	using NthType = typename std::tuple_element<i, std::tuple<Types...>>::type;
}
