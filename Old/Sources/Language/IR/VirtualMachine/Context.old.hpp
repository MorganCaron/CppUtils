#pragma once

#include <CppUtils/Language/IR/VirtualMachine/ProgramMemory.hpp>

namespace CppUtils::Language::IR::VirtualMachine
{
	template<std::size_t stackSize>
	struct Context final
	{
		Compiler::Output compilerOutput;
		ProgramMemory<stackSize> programMemory;
	};
}
