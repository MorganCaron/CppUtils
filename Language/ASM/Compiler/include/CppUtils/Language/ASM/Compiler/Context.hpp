#pragma once

#include <CppUtils/Language/Compiler/Context.hpp>
#include <CppUtils/Language/ASM/Compiler/Bytecode.hpp>

namespace CppUtils::Language::ASM::Compiler
{
	template<typename... Types>
	using Context = Language::Compiler::Context<Parser::ASTNode<Type::Token, Types...>, Bytecode::Instruction<Types...>>;
}
