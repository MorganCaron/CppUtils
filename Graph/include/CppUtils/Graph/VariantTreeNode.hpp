#pragma once

#include <variant>
#include <concepts>
#include <type_traits>

#include <CppUtils/Graph/TreeNode.hpp>
#include <CppUtils/Type/VariadicTemplate.hpp>

namespace CppUtils::Graph
{
	template<typename... Types> requires std::is_default_constructible_v<VariadicTemplate::NthType<0, Types...>>
	using VariantTreeNode = TreeNode<std::variant<Types...>>;
}
