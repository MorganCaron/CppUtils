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
				{ "ret"_token, Operations<Instruction, Context, Address>::runRet },
				{ "call"_token, Operations<Instruction, Context, Address>::runCall }/*,
				{ "ifnz"_token, Operations<Instruction, Context, Address>::runIf }*/
			}}
		{}

		template<typename ReturnType> requires std::is_default_constructible_v<ReturnType>
		[[nodiscard]] ReturnType run(const Type::Token& label, Context& context) const
		{
			context.jump(label);
			context.push(ReturnType{});
			while (context.registerVariables.eip != 0)
			{
				const auto& instruction = *reinterpret_cast<const Instruction*>(context.registerVariables.eip);
				m_virtualMachine.run(instruction.type, instruction, context);
			}
			return context.template pop<ReturnType>();
		}

		template<typename ReturnType> requires std::is_default_constructible_v<ReturnType>
		[[nodiscard]] ReturnType run(const Type::Token& label, std::string_view src, Context& context) const
		{
			static const auto compiler = Compiler::Compiler<Address>{};
			context.compilerOutput = compiler.compile(src);
			return run<ReturnType>(label, context);
		}

	private:
		Language::VirtualMachine::VirtualMachine<Instruction, Context> m_virtualMachine;
	};
}
