#pragma once

#include <memory>
#include <functional>
#include <unordered_map>

#include <CppUtils/Language/IR/Lexer/Lexer.hpp>
#include <CppUtils/Language/IR/Compiler/Output.hpp>
#include <CppUtils/Language/Compiler/Compiler.hpp>

namespace CppUtils::Language::IR::Compiler
{
	struct Context final
	{
		using Compiler = Language::Compiler::Compiler<Lexer::ASTNode, Context>;

		std::reference_wrapper<const Compiler> compiler;
		std::reference_wrapper<Output> output;
		std::unordered_map<Type::Token, std::uintptr_t, Type::Token::hash_fn> variablesToRegisters = {};
		Bytecode::Instruction* lastInstruction = nullptr;
		std::uintptr_t registerCounter = 0;
		std::uintptr_t returnRegister = 0;

		explicit Context(const Compiler& c_compiler, Output& c_output):
			compiler{c_compiler}, output{c_output}
		{}

		template<typename... Args>
		Bytecode::Instruction* createInstruction(Args... args)
		{
			return output.get().instructions.emplace_back(std::make_unique<Bytecode::Instruction>(std::forward<Args>(args)...)).get();
		}

		void addInstruction(Bytecode::Instruction* instruction) noexcept
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
			Context::output.get().functions[label] = FunctionInformations{lastInstruction, nbParameters};
		}

		[[nodiscard]] std::uintptr_t newRegister() noexcept
		{
			return registerCounter++;
		}

		[[nodiscard]] std::uintptr_t getRegister(const Type::Token& variable)
		{
			if (variablesToRegisters.find(variable) == variablesToRegisters.end())
				variablesToRegisters[variable] = newRegister();
			return variablesToRegisters.at(variable);
		}
	};
}
