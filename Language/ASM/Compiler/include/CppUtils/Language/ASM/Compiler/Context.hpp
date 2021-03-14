#pragma once

#include <memory>
#include <unordered_map>

#include <CppUtils/Language/Compiler/Context.hpp>
#include <CppUtils/Language/ASM/Compiler/Bytecode.hpp>

namespace CppUtils::Language::ASM::Compiler
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
		Bytecode::Instruction<Address>* lastInstruction = nullptr;
		std::size_t registerCounter = 0;
		Address returnRegister = 0;

		explicit Context(std::reference_wrapper<const Compiler<Address>> compiler):
			Language::Compiler::Context<Compiler<Address>, Bytecode::Instruction<Address>>{compiler}
		{}

		inline void addInstruction(Bytecode::Instruction<Address>* instruction)
		{
			if (lastInstruction == nullptr)
				lastInstruction = instruction;
			while (lastInstruction->nextInstruction != nullptr)
				lastInstruction = lastInstruction->nextInstruction;
		}

		[[nodiscard]] inline Address newRegister() noexcept
		{
			return registerCounter++;
		}
	};
}
