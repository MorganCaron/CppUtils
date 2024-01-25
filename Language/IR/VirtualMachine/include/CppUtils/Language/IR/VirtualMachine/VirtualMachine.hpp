#pragma once

#include <functional>
#include <stack>
#include <type_traits>

#include <CppUtils/Language/IR/Compiler/Compiler.hpp>
#include <CppUtils/Language/IR/VirtualMachine/Context.hpp>
#include <CppUtils/Language/IR/VirtualMachine/Operations.hpp>
#include <CppUtils/Language/VirtualMachine/VirtualMachine.hpp>

namespace CppUtils::Language::IR::VirtualMachine
{
	using namespace Type::Literals;

	template<std::size_t stackSize>
	class VirtualMachine final
	{
	public:
		using Instruction = Compiler::Bytecode::Instruction;

		VirtualMachine():
			m_virtualMachine{{
				{"nop"_token, Operations<Context<stackSize>>::runNop},
				{"halt"_token, Operations<Context<stackSize>>::runHalt},
				{"init"_token, Operations<Context<stackSize>>::runInit},
				{"read"_token, Operations<Context<stackSize>>::runRead},
				{"write"_token, Operations<Context<stackSize>>::runWrite},
				{"copy"_token, Operations<Context<stackSize>>::runCopy},
				/*{ "eq"_token, Operations<Context<stackSize>>::runEq },*/
				{"add"_token, Operations<Context<stackSize>>::runAdd},
				{"sub"_token, Operations<Context<stackSize>>::runSub},
				{"call"_token, Operations<Context<stackSize>>::runCall},
				{"ret"_token, Operations<Context<stackSize>>::runRet} /*,
			   { "ifnz"_token, Operations<Context<stackSize>>::runIf }*/
			}}
		{}

		[[nodiscard]] std::uintptr_t run(const Type::Token& label, Context<stackSize>& context) const
		{
			auto& [compilerOutput, programMemory] = context;
			programMemory.template push<std::uintptr_t>(0);
			programMemory.registerFile[0] = programMemory.registerVariables.stackPointer;
			programMemory.call(reinterpret_cast<std::uintptr_t>(compilerOutput.getFunctionEntryPoint(label)));
			programMemory.enter(0);
			while (programMemory.registerVariables.instructionPointer != 0)
				m_virtualMachine.run(programMemory.getInstruction().type, programMemory.getInstruction(), context);
			return programMemory.template pop<std::uintptr_t>();
		}

		[[nodiscard]] std::uintptr_t run(const Type::Token& label, std::string_view src, Context<stackSize>& context) const
		{
			static const auto compiler = Compiler::Compiler{};
			context.compilerOutput = compiler.compile(src);
			return run(label, context);
		}

	private:
		Language::VirtualMachine::VirtualMachine<Instruction, Context<stackSize>> m_virtualMachine;
	};
}
