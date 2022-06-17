#pragma once

#include <variant>

#include <CppUtils/Type/Concept.hpp>
#include <CppUtils/Graph/TreeNode.hpp>
#include <CppUtils/Type/VariadicTemplate.hpp>

namespace CppUtils::Graph
{
	template<class... Types>
	requires std::default_initializable<Type::VariadicTemplate::NthType<0, Types...>>
	using VariantTreeNode = TreeNode<std::variant<Types...>>;
}
