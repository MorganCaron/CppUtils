#pragma once

#include <memory>
#include <functional>
#include <unordered_map>

#include <CppUtils/Language/IR/Lexer/Lexer.hpp>
#include <CppUtils/Language/IR/Compiler/Output.hpp>

namespace CppUtils::Language::IR::Compiler
{
	template<typename Address>
	requires Type::Traits::isAddress<Address>
	class Compiler;
	
	template<typename Address>
	requires Type::Traits::isAddress<Address>
	struct Context final
	{
		std::reference_wrapper<const Compiler<Address>> compiler;
		std::reference_wrapper<Output<Address>> output;
		std::unordered_map<Type::Token, Address, Type::Token::hash_fn> variablesToRegisters = {};
		Bytecode::Instruction<Address>* lastInstruction = nullptr;
		Address registerCounter = 0;
		Address returnRegister = 0;

		explicit Context(const Compiler<Address>& c_compiler, Output<Address>& c_output):
			compiler{c_compiler}, output{c_output}
		{}

		template<typename... Args>
		Bytecode::Instruction<Address>* createInstruction(Args... args)
		{
			return output.get().instructions.emplace_back(std::make_unique<Bytecode::Instruction<Address>>(std::forward<Args>(args)...)).get();
		}

		void addInstruction(Bytecode::Instruction<Address>* instruction) noexcept
		{
			if (lastInstruction == nullptr)
				lastInstruction = instruction;
			else
				lastInstruction->nextInstruction = instruction;
			while (lastInstruction->nextInstruction != nullptr)
				lastInstruction = lastInstruction->nextInstruction;
		}

		void addFunction(const Type::Token& label, std::size_t nbParameters = 0)
		{
			if (Context::output.get().functions.find(label) != Context::output.get().functions.end())
				throw std::runtime_error{"Function " + std::string{label.name} + " is already defined."};
			addInstruction(createInstruction());
			Context::output.get().functions[label] = FunctionInformations<Address>{lastInstruction, nbParameters};
		}

		[[nodiscard]] Address newRegister() noexcept
		{
			return registerCounter++;
		}

		[[nodiscard]] Address getRegister(const Type::Token& variable)
		{
			if (variablesToRegisters.find(variable) == variablesToRegisters.end())
				variablesToRegisters[variable] = newRegister();
			return variablesToRegisters.at(variable);
		}
	};
}
