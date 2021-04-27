#pragma once

#include <list>
#include <memory>
#include <unordered_map>

#include <CppUtils/Language/Compiler/Context.hpp>
#include <CppUtils/Language/IR/Compiler/Output.hpp>

namespace CppUtils::Language::IR::Compiler
{
	template<typename Address>
	requires Type::Traits::isAddress<Address>
	class Compiler;
	
	template<typename Address>
	requires Type::Traits::isAddress<Address>
	struct Context final: public Language::Compiler::Context<Compiler<Address>, Bytecode::Instruction<Address>, Output<Address>>
	{
		std::unordered_map<Type::Token, Address, Type::Token::hash_fn> variablesToRegisters = {};
		Bytecode::Instruction<Address>* lastInstruction = nullptr;
		Address registerCounter = 0;
		Address returnRegister = 0;

		explicit Context(std::reference_wrapper<const Compiler<Address>> compiler):
			Language::Compiler::Context<Compiler<Address>, Bytecode::Instruction<Address>, Output<Address>>{compiler}
		{}

		void addFunction(const Type::Token& label, std::size_t nbParameters = 0)
		{
			if (Context::output.functions.find(label) != Context::output.functions.end())
				throw std::runtime_error{"Function " + std::string{label.name} + " is already defined."};
			addInstruction(Language::Compiler::Context<Compiler<Address>, Bytecode::Instruction<Address>, Output<Address>>::createInstruction());
			Context::output.functions[label] = FunctionInformations<Address>{lastInstruction, nbParameters};
		}

		void addInstruction(Bytecode::Instruction<Address>* instruction)
		{
			if (lastInstruction == nullptr)
				lastInstruction = instruction;
			else
				lastInstruction->nextInstruction = instruction;
			while (lastInstruction->nextInstruction != nullptr)
				lastInstruction = lastInstruction->nextInstruction;
		}

		[[nodiscard]] inline Address newRegister() noexcept
		{
			return registerCounter++;
		}

		[[nodiscard]] inline Address getRegister(const Type::Token& variable)
		{
			if (variablesToRegisters.find(variable) == variablesToRegisters.end())
				variablesToRegisters[variable] = newRegister();
			return variablesToRegisters.at(variable);
		}
	};
}
