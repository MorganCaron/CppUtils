#pragma once

#include <array>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>

#include <CppUtils/Language/IR/Compiler/Bytecode.hpp>

namespace CppUtils::Language::IR::VirtualMachine
{
	template<std::size_t stackSize>
	struct RegisterVariables final
	{
		std::uintptr_t instructionPointer = 0; // eip
		std::uintptr_t basePointer = stackSize; // ebp (32bits) / rbp (64bits)
		std::uintptr_t stackPointer = stackSize; // esp (32bits) / rsp (64bits)
	};

	template<std::size_t stackSize>
	struct ProgramMemory final
	{
		RegisterVariables<stackSize> registerVariables;
		std::array<std::uintptr_t, stackSize> programMemory;
		std::unordered_map<std::size_t, std::uintptr_t> registerFile;

		template<class T>
		[[nodiscard]] constexpr std::size_t countNecessaryMemoryCells() const noexcept
		{
			return static_cast<std::size_t>(std::ceil(sizeof(T) / sizeof(std::uintptr_t)));
		}

		template<class T>
		[[nodiscard]] const T& getStackAddress(std::uintptr_t address) const noexcept
		{
			return *reinterpret_cast<const T*>(programMemory.data() + address);
		}

		template<class T>
		[[nodiscard]] T& getStackAddress(std::uintptr_t address) noexcept
		{
			return *reinterpret_cast<T*>(programMemory.data() + address);
		}

		void halt() noexcept
		{
			jump(0);
		}

		void jump(std::uintptr_t destination) noexcept
		{
			registerVariables.instructionPointer = destination;
		}

		template<class T>
		void push(T value)
		{
			if (registerVariables.stackPointer < countNecessaryMemoryCells<T>())
				throw std::runtime_error{"Stack overflow"};
			registerVariables.stackPointer -= countNecessaryMemoryCells<T>();
			getStackAddress<T>(registerVariables.stackPointer) = std::move(value);
		}

		template<class T>
		T pop()
		{
			if (registerVariables.stackPointer + countNecessaryMemoryCells<T>() > stackSize)
				throw std::runtime_error{"Stack underflow"};
			T value = getStackAddress<T>(registerVariables.stackPointer);
			registerVariables.stackPointer += countNecessaryMemoryCells<T>();
			return value;
		}

		template<class T>
		[[nodiscard]] bool isWrongMemoryAddress(std::uintptr_t address) const noexcept
		{
			return (address < registerVariables.stackPointer or address + countNecessaryMemoryCells<T>() > stackSize);
		}

		template<class T>
		[[nodiscard]] const T& get(std::uintptr_t address) const
		{
			if (isWrongMemoryAddress<T>(address))
				throw std::runtime_error{"Segmentation fault"};
			return getStackAddress<T>(address);
		}

		template<class T>
		void set(std::uintptr_t address, T value)
		{
			if (isWrongMemoryAddress<T>(address))
				throw std::runtime_error{"Segmentation fault"};
			getStackAddress<T>(address) = std::move(value);
		}

		template<class T>
		[[nodiscard]] const T& top() const
		{
			return get<T>(registerVariables.stackPointer);
		}

		void call(std::uintptr_t destination)
		{
			push<std::uintptr_t>(registerVariables.instructionPointer == 0 ? 0 : reinterpret_cast<std::uintptr_t>(getInstruction().nextInstruction));
			jump(destination);
		}

		// Function prologue
		void enter(std::size_t argumentsStorage)
		{
			push<std::uintptr_t>(registerVariables.basePointer);
			registerVariables.basePointer = registerVariables.stackPointer;
			registerVariables.stackPointer -= argumentsStorage;
		}

		// Function epilogue
		void leave()
		{
			registerVariables.stackPointer = registerVariables.basePointer;
			registerVariables.basePointer = pop<std::uintptr_t>();
		}

		void ret()
		{
			jump(pop<std::uintptr_t>());
		}

		template<class T>
		void ret(T result)
		{
			ret();
			set<T>(registerVariables.stackPointer, std::move(result));
		}

		[[nodiscard]] const Compiler::Bytecode::Instruction& getInstruction() const
		{
			if (registerVariables.instructionPointer == 0)
				throw std::runtime_error{"Instruction not found"};
			return *reinterpret_cast<Compiler::Bytecode::Instruction*>(registerVariables.instructionPointer);
		}

		void goToNextInstruction()
		{
			jump(reinterpret_cast<std::uintptr_t>(getInstruction().nextInstruction));
		}
	};
}
