#pragma once

#include <CppUtils/Type/Token.hpp>
#include <CppUtils/Graph/VariantTreeNode.hpp>

namespace CppUtils::Language::Compiler::IR
{
	using Value = std::variant<Type::Token, unsigned int>;
	using TreeNode = Graph::VariantTreeNode<Type::Token, unsigned int>;
}
