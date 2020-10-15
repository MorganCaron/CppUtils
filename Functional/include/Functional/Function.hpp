#pragma once

#include <vector>
#include <utility>
#include <functional>

namespace CppUtils::Functional
{
	template<std::size_t... S, typename Argument, typename Return, typename... Arguments>
	inline Return callFunction(const std::function<Return(Arguments...)>& function, const std::vector<Argument>& vec, std::index_sequence<S...>)
	{
		return function(vec[S]...);
	}

	template<std::size_t size, typename Argument, typename Return, typename... Arguments>
	inline Return callFunction(const std::function<Return(Arguments...)>& function, const std::vector<Argument>& vec)
	{
		if (vec.size() != size)
			throw std::invalid_argument{"Incorrect number of arguments"};
		return callFunction(function, vec, std::make_index_sequence<size>());
	}
}
