#pragma once

#include <array>
#include <cstddef>
#include <concepts>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>

#include <CppUtils/Language/IR/Compiler/Bytecode.hpp>

namespace CppUtils::Language::IR::VirtualMachine
{
	template<typename Address>
	requires Type::Traits::isAddress<Address>
	struct RegisterVariables final
	{
		Address eip; // Instruction Pointer
		Address ebp; // Base Pointer
		Address esp; // Stack Pointer
	};
	
	template<typename Address, std::size_t stackSize>
	requires Type::Traits::isAddress<Address>
	struct Context final
	{
		Compiler::Output<Address> compilerOutput;
		std::unordered_map<std::size_t, Address> registerFile;
		RegisterVariables<Address> registerVariables;
		std::array<Address, stackSize> programMemory;

		inline void halt() noexcept
		{
			jump(0);
		}

		inline void jump(Address destination) noexcept
		{
			registerVariables.eip = destination;
		}

		void jump(const Type::Token& label)
		{
			if (compilerOutput.functions.find(label) == compilerOutput.functions.end())
				throw std::runtime_error{"Undefined label " + std::string{label.name}};
			jump(reinterpret_cast<Address>(compilerOutput.functions.at(label).entryPoint));
		}

		template<typename T>
		inline void push(T value)
		{
			if (registerVariables.esp + sizeof(T) > stackSize)
				throw std::runtime_error{"Stack overflow"};
			programMemory[registerVariables.esp] = value;
			registerVariables.esp += sizeof(T);
		}

		template<typename T>
		inline T pop()
		{
			if (registerVariables.esp - sizeof(T) < registerVariables.ebp)
				throw std::runtime_error{"Stack underflow"};
			T value = programMemory.at(registerVariables.esp);
			registerVariables.esp -= sizeof(T);
			return value;
		}

		[[nodiscard]] inline const Compiler::Bytecode::Instruction<Address>& getInstruction() const
		{
			if (registerVariables.eip == 0)
				throw std::runtime_error{"Instruction not found"};
			return *reinterpret_cast<Compiler::Bytecode::Instruction<Address>*>(registerVariables.eip);
		}

		inline void goToNextInstruction()
		{
			jump(reinterpret_cast<Address>(getInstruction().nextInstruction));
		}
	};
}
