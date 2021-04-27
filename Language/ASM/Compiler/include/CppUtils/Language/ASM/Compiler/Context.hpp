#pragma once

#include <memory>
#include <unordered_map>

#include <CppUtils/Language/Compiler/Context.hpp>
#include <CppUtils/Language/ASM/Compiler/Output.hpp>

namespace CppUtils::Language::ASM::Compiler
{
	template<typename Address>
	requires Type::Traits::isAddress<Address>
	class Compiler;

	template<typename Address>
	requires Type::Traits::isAddress<Address>
	struct Context final: public Language::Compiler::Context<Compiler<Address>, Bytecode::Instruction<Address>, Output<Address>>
	{
		Bytecode::Instruction<Address>* lastInstruction = nullptr;
		std::size_t registerCounter = 0;
		Address returnRegister = 0;

		explicit Context(std::reference_wrapper<const Compiler<Address>> compiler):
			Language::Compiler::Context<Compiler<Address>, Bytecode::Instruction<Address>, Output<Address>>{compiler}
		{}

		inline void addInstruction(Bytecode::Instruction<Address>* instruction)
		{
			if (lastInstruction == nullptr)
				lastInstruction = instruction;
			while (lastInstruction->nextInstruction != nullptr)
				lastInstruction = lastInstruction->nextInstruction;
		}

		[[nodiscard]] inline Address newRegister() noexcept
		{
			return registerCounter++;
		}
	};
}
