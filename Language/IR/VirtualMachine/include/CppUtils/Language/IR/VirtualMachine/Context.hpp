#pragma once

#include <CppUtils/Language/IR/VirtualMachine/ProgramMemory.hpp>

namespace CppUtils::Language::IR::VirtualMachine
{
	template<typename Address, std::size_t stackSize>
	requires Type::Traits::isAddress<Address>
	struct Context final
	{
		Compiler::Output<Address> compilerOutput;
		ProgramMemory<Address, stackSize> programMemory;
	};
}
