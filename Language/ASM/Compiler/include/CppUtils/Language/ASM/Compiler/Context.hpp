#pragma once

#include <memory>
#include <unordered_map>

#include <CppUtils/Language/ASM/Compiler/Bytecode.hpp>

namespace CppUtils::Language::ASM::Compiler
{
	template<typename Type>
	struct FunctionInformations final
	{
		Bytecode::Instruction<Type>* entryPoint;
		std::size_t nbParameters;
	};

	template<typename Type>
	struct Context final: public Language::Compiler::Context<Bytecode::Instruction<Type>>
	{
		std::unordered_map<CppUtils::Type::Token, FunctionInformations<Type>, CppUtils::Type::Token::hash_fn> functions;
		Bytecode::Instruction<Type>* lastInstruction;
		std::size_t registerCounter;

		inline void addInstruction(Bytecode::Instruction<Type>* instruction)
		{
			if (lastInstruction == nullptr)
				lastInstruction = instruction;
			while (lastInstruction->nextInstruction != nullptr)
				lastInstruction = lastInstruction->nextInstruction;
		}
	};
}
