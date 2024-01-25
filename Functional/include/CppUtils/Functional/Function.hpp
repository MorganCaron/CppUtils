#pragma once

#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace CppUtils::Functional
{
	template<std::size_t... S, class Return, class... Arguments, class Argument>
	Return callFunction(const std::function<Return(Arguments...)>& function, const std::vector<Argument>& arguments, std::index_sequence<S...>)
	{
		return function(arguments[S]...);
	}

	template<std::size_t size, class Return, class... Arguments, class Argument>
	Return callFunction(const std::function<Return(Arguments...)>& function, const std::vector<Argument>& arguments)
	{
		if (arguments.size() != size)
			throw std::invalid_argument{"Incorrect number of arguments"};
		return callFunction(function, arguments, std::make_index_sequence<size>());
	}
}
