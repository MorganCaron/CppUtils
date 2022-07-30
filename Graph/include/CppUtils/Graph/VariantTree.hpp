#pragma once

#include <variant>

#include <CppUtils/Type/Concept.hpp>
#include <CppUtils/Type/VariadicTemplate.hpp>
#include <CppUtils/Graph/Tree.hpp>

namespace CppUtils::Graph::Tree
{
	template<class... Types>
	requires std::default_initializable<Type::VariadicTemplate::NthType<0, Types...>>
	using VariantNode = Node<std::variant<Types...>>;
}
