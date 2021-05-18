#pragma once

#include <memory>
#include <functional>
#include <unordered_map>

#include <CppUtils/Language/ASM/Lexer/Lexer.hpp>
#include <CppUtils/Language/ASM/Compiler/Output.hpp>

namespace CppUtils::Language::ASM::Compiler
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
		Bytecode::Instruction<Address>* lastInstruction = nullptr;
		std::size_t registerCounter = 0;
		Address returnRegister = 0;

		explicit Context(const Compiler<Address>& c_compiler, Output<Address>& c_output):
			compiler{c_compiler}, output{c_output}
		{}

		template<typename... Args>
		inline Bytecode::Instruction<Address>* createInstruction(Args... args)
		{
			return Context::output.get().instructions.emplace_back(std::make_unique<Bytecode::Instruction<Address>>(std::forward<Args>(args)...)).get();
		}

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
