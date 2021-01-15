#pragma once

#include <CppUtils/Language/Compiler/Context.hpp>
#include <CppUtils/ASM/Compiler/Bytecode.hpp>

namespace CppUtils::ASM::Compiler
{
	template<typename... Types>
	using Context = Language::Compiler::Context<Graph::VariantTreeNode<Type::Token, Types...>, Bytecode::Instruction<Types...>>;
}
