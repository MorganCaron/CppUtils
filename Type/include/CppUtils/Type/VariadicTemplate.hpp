#pragma once

#include <tuple>

namespace CppUtils::VariadicTemplate
{
	template<std::size_t i, typename...Types>
	using NthType = typename std::tuple_element<i, std::tuple<Types...>>::type;
}
