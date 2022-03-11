#pragma once

#include <cmath>
#include <array>
#include <cstddef>
#include <concepts>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>

#include <CppUtils/Language/IR/Compiler/Bytecode.hpp>

namespace CppUtils::Language::IR::VirtualMachine
{
	template<typename Address, std::size_t stackSize>
	requires Type::Traits::isAddress<Address>
	struct RegisterVariables final
	{
		Address instructionPointer = 0; // eip
		Address basePointer = stackSize; // ebp (32bits) / rbp (64bits)
		Address stackPointer = stackSize; // esp (32bits) / rsp (64bits)
	};
	
	template<typename Address, std::size_t stackSize>
	requires Type::Traits::isAddress<Address>
	struct ProgramMemory final
	{
		RegisterVariables<Address, stackSize> registerVariables;
		std::array<Address, stackSize> programMemory;
		std::unordered_map<std::size_t, Address> registerFile;

		template<typename T>
		[[nodiscard]] constexpr std::size_t countNecessaryMemoryCells() const noexcept
		{
			return static_cast<std::size_t>(std::ceil(sizeof(T) / sizeof(Address)));
		}

		template<typename T>
		[[nodiscard]] const T& getStackAddress(Address address) const noexcept
		{
			return *reinterpret_cast<const T*>(programMemory.data() + address);
		}

		template<typename T>
		[[nodiscard]] T& getStackAddress(Address address) noexcept
		{
			return *reinterpret_cast<T*>(programMemory.data() + address);
		}

		void halt() noexcept
		{
			jump(0);
		}

		void jump(Address destination) noexcept
		{
			registerVariables.instructionPointer = destination;
		}

		template<typename T>
		void push(T value)
		{
			if (registerVariables.stackPointer < countNecessaryMemoryCells<T>())
				throw std::runtime_error{"Stack overflow"};
			registerVariables.stackPointer -= countNecessaryMemoryCells<T>();
			getStackAddress<T>(registerVariables.stackPointer) = std::move(value);
		}

		template<typename T>
		T pop()
		{
			if (registerVariables.stackPointer + countNecessaryMemoryCells<T>() > stackSize)
				throw std::runtime_error{"Stack underflow"};
			T value = getStackAddress<T>(registerVariables.stackPointer);
			registerVariables.stackPointer += countNecessaryMemoryCells<T>();
			return value;
		}

		template<typename T>
		[[nodiscard]] bool isWrongMemoryAddress(Address address) const noexcept
		{
			return (address < registerVariables.stackPointer || address + countNecessaryMemoryCells<T>() > stackSize);
		}

		template<typename T>
		[[nodiscard]] const T& get(Address address) const
		{
			if (isWrongMemoryAddress<T>(address))
				throw std::runtime_error{"Segmentation fault"};
			return getStackAddress<T>(address);
		}

		template<typename T>
		void set(Address address, T value)
		{
			if (isWrongMemoryAddress<T>(address))
				throw std::runtime_error{"Segmentation fault"};
			getStackAddress<T>(address) = std::move(value);
		}

		template<typename T>
		[[nodiscard]] const T& top() const
		{
			return get<T>(registerVariables.stackPointer);
		}

		void call(Address destination)
		{
			push<Address>(registerVariables.instructionPointer == 0 ? 0 : reinterpret_cast<Address>(getInstruction().nextInstruction));
			jump(destination);
		}

		// Function prologue
		void enter(std::size_t argumentsStorage)
		{
			push<Address>(registerVariables.basePointer);
			registerVariables.basePointer = registerVariables.stackPointer;
			registerVariables.stackPointer -= argumentsStorage;
		}

		// Function epilogue
		void leave()
		{
			registerVariables.stackPointer = registerVariables.basePointer;
			registerVariables.basePointer = pop<Address>();
		}

		void ret()
		{
			jump(pop<Address>());
		}

		template<typename T>
		void ret(T result)
		{
			ret();
			set<T>(registerVariables.stackPointer, std::move(result));
		}

		[[nodiscard]] const Compiler::Bytecode::Instruction<Address>& getInstruction() const
		{
			if (registerVariables.instructionPointer == 0)
				throw std::runtime_error{"Instruction not found"};
			return *reinterpret_cast<Compiler::Bytecode::Instruction<Address>*>(registerVariables.instructionPointer);
		}

		void goToNextInstruction()
		{
			jump(reinterpret_cast<Address>(getInstruction().nextInstruction));
		}
	};
}
