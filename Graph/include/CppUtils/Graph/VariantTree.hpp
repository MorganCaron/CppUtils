#pragma once

#include <variant>

#include <CppUtils/Graph/Tree.hpp>
#include <CppUtils/Type/Concept.hpp>
#include <CppUtils/Type/VariadicTemplate.hpp>

namespace CppUtils::Graph::Tree
{
	template<class... Types>
	requires std::default_initializable<Type::NthType<0, Types...>>
	using VariantNode = Node<std::variant<Types...>>;
}
