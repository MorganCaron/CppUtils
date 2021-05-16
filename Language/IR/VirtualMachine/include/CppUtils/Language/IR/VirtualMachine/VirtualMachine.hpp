#pragma once

#include <stack>
#include <type_traits>
#include <functional>

#include <CppUtils/Language/IR/Compiler/Compiler.hpp>
#include <CppUtils/Language/IR/VirtualMachine/Context.hpp>
#include <CppUtils/Language/IR/VirtualMachine/Operations.hpp>
#include <CppUtils/Language/VirtualMachine/VirtualMachine.hpp>

namespace CppUtils::Language::IR::VirtualMachine
{
	using namespace Type::Literals;

	template<typename Address, std::size_t stackSize>
	requires Type::Traits::isAddress<Address>
	class VirtualMachine final
	{
	public:
		using Instruction = Compiler::Bytecode::Instruction<Address>;
		using Context = Context<Address, stackSize>;

		VirtualMachine(): m_virtualMachine{{
				{ "nop"_token, Operations<Instruction, Context, Address>::runNop },
				{ "halt"_token, Operations<Instruction, Context, Address>::runHalt },
				{ "init"_token, Operations<Instruction, Context, Address>::runInit },
				{ "read"_token, Operations<Instruction, Context, Address>::runRead },
				{ "write"_token, Operations<Instruction, Context, Address>::runWrite },
				{ "copy"_token, Operations<Instruction, Context, Address>::runCopy },
				/*{ "eq"_token, Operations<Instruction, Context, Address>::runEq },*/
				{ "add"_token, Operations<Instruction, Context, Address>::runAdd },
				{ "sub"_token, Operations<Instruction, Context, Address>::runSub },
				{ "call"_token, Operations<Instruction, Context, Address>::runCall },
				{ "ret"_token, Operations<Instruction, Context, Address>::runRet }/*,
				{ "ifnz"_token, Operations<Instruction, Context, Address>::runIf }*/
			}}
		{}

		[[nodiscard]] Address run(const Type::Token& label, Context& context) const
		{
			auto& [compilerOutput, programMemory] = context;
			programMemory.template push<Address>(0);
			programMemory.registerFile[0] = programMemory.registerVariables.stackPointer;
			programMemory.call(reinterpret_cast<Address>(compilerOutput.getFunctionEntryPoint(label)));
			programMemory.enter(0);
			while (programMemory.registerVariables.instructionPointer != 0)
				m_virtualMachine.run(programMemory.getInstruction().type, programMemory.getInstruction(), context);
			return programMemory.template pop<Address>();
		}

		[[nodiscard]] Address run(const Type::Token& label, std::string_view src, Context& context) const
		{
			static const auto compiler = Compiler::Compiler<Address>{};
			context.compilerOutput = compiler.compile(src);
			return run(label, context);
		}

	private:
		Language::VirtualMachine::VirtualMachine<Instruction, Context> m_virtualMachine;
	};
}
