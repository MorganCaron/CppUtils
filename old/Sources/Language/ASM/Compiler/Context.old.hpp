#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

#include <CppUtils/Language/ASM/Compiler/Output.hpp>
#include <CppUtils/Language/ASM/Lexer/Lexer.hpp>
#include <CppUtils/Language/Compiler/Compiler.hpp>

namespace CppUtils::Language::ASM::Compiler
{
	struct Context final
	{
		using Compiler = Language::Compiler::Compiler<Lexer::ASTNode, Context>;

		std::reference_wrapper<const Compiler> compiler;
		std::reference_wrapper<Output> output;
		Bytecode::Instruction* lastInstruction = nullptr;
		std::size_t registerCounter = 0;
		std::uintptr_t returnRegister = 0;

		explicit Context(const Compiler& c_compiler, Output& c_output):
			compiler{c_compiler}, output{c_output}
		{}

		template<class... Args>
		Bytecode::Instruction* createInstruction(Args... args)
		{
			return Context::output.get().instructions.emplace_back(std::make_unique<Bytecode::Instruction>(std::forward<Args>(args)...)).get();
		}

		void addInstruction(Bytecode::Instruction* instruction)
		{
			if (lastInstruction == nullptr)
				lastInstruction = instruction;
			while (lastInstruction->nextInstruction != nullptr)
				lastInstruction = lastInstruction->nextInstruction;
		}

		[[nodiscard]] std::uintptr_t newRegister() noexcept
		{
			return registerCounter++;
		}
	};
}
