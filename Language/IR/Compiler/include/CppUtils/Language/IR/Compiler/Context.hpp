#pragma once

#include <memory>
#include <unordered_map>

#include <CppUtils/Language/Compiler/Context.hpp>
#include <CppUtils/Language/IR/Compiler/Bytecode.hpp>

namespace CppUtils::Language::IR::Compiler
{
	template<typename Address> requires std::is_integral_v<Address>
	struct FunctionInformations final
	{
		Bytecode::Instruction<Address>* entryPoint = nullptr;
		std::size_t nbParameters = 0;
	};

	template<typename Address> requires std::is_integral_v<Address>
	class Compiler;
	template<typename Address> requires std::is_integral_v<Address>
	struct Context final: public Language::Compiler::Context<Compiler<Address>, Bytecode::Instruction<Address>>
	{
		std::unordered_map<Type::Token, FunctionInformations<Address>, Type::Token::hash_fn> functions = {};
		std::unordered_map<Type::Token, Address, Type::Token::hash_fn> variablesToRegisters = {};
		Bytecode::Instruction<Address>* lastInstruction = nullptr;
		std::size_t registerCounter = 0;
		Address returnRegister = 0;

		explicit Context(std::reference_wrapper<const Compiler<Address>> compiler):
			Language::Compiler::Context<Compiler<Address>, Bytecode::Instruction<Address>>{compiler}
		{}

		void addFunction(const Type::Token& label, std::size_t nbParameters = 0)
		{
			if (functions.find(label) != functions.end())
				throw std::runtime_error{"Function " + std::string{label.name} + " is already defined."};
			addInstruction(Language::Compiler::Context<Compiler<Address>, Bytecode::Instruction<Address>>::createInstruction());
			functions[label] = FunctionInformations<Address>{lastInstruction, nbParameters};
		}

		inline void addInstruction(Bytecode::Instruction<Address>* instruction)
		{
			if (lastInstruction == nullptr) lastInstruction = instruction;
			while (lastInstruction->nextInstruction != nullptr)
				lastInstruction = lastInstruction->nextInstruction;
		}

		[[nodiscard]] inline Address newRegister() noexcept
		{
			return static_cast<Address>(registerCounter++);
		}

		[[nodiscard]] inline Address getRegister(const Type::Token& variable)
		{
			if (variablesToRegisters.find(variable) == variablesToRegisters.end())
				variablesToRegisters[variable] = newRegister();
			return variablesToRegisters.at(variable);
		}
	};
}
