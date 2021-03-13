#pragma once

#include <memory>
#include <unordered_map>

#include <CppUtils/Language/ASM/Compiler/Bytecode.hpp>

namespace CppUtils::Language::ASM::Compiler
{
	template<typename Type>
	struct FunctionInformations final
	{
		Bytecode::Instruction<Type>* entryPoint = nullptr;
		std::size_t nbParameters = 0;
	};

	template<typename Type> class Compiler;
	template<typename Type>
	struct Context final: public Language::Compiler::Context<Compiler<Type>, Bytecode::Instruction<Type>>
	{
		std::unordered_map<CppUtils::Type::Token, FunctionInformations<Type>, CppUtils::Type::Token::hash_fn> functions = {};
		Bytecode::Instruction<Type>* lastInstruction = nullptr;
		std::size_t registerCounter = 0;
		std::size_t returnRegister = 0;

		explicit Context(std::reference_wrapper<const Compiler<Type>> compiler):
			Language::Compiler::Context<Compiler<Type>, Bytecode::Instruction<Type>>{compiler}
		{}

		inline void addInstruction(Bytecode::Instruction<Type>* instruction)
		{
			if (lastInstruction == nullptr)
				lastInstruction = instruction;
			while (lastInstruction->nextInstruction != nullptr)
				lastInstruction = lastInstruction->nextInstruction;
		}

		[[nodiscard]] inline std::size_t newRegister() noexcept
		{
			return registerCounter++;
		}
	};
}
