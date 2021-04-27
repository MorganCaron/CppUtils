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
	class VirtualMachine final
	{
	public:
		using Instruction = Compiler::Bytecode::Instruction<Address>;
		using Context = Context<Address, stackSize>;

		VirtualMachine(): m_virtualMachine{{
				{ "nop"_token, Operations<Context>::runNop },
				{ "halt"_token, Operations<Context>::runHalt },
				{ "init"_token, Operations<Context>::runInit },
				{ "read"_token, Operations<Context>::runRead },
				{ "write"_token, Operations<Context>::runWrite },
				{ "copy"_token, Operations<Context>::runCopy },
				/*{ "eq"_token, Operations<Context>::runEq },*/
				{ "add"_token, Operations<Context>::runAdd },
				{ "sub"_token, Operations<Context>::runSub },
				{ "ret"_token, Operations<Context>::runRet },
				{ "call"_token, Operations<Context>::runCall }/*,
				{ "ifnz"_token, Operations<Context>::runIf }*/
			}}
		{}

		template<typename ReturnType> requires std::is_default_constructible_v<ReturnType>
		[[nodiscard]] ReturnType run([[maybe_unused]] Context& context) const
		{
			/*
			context.stack.push(ReturnType{});
			m_virtualMachine.run(context);
			return context.pop();
			*/
			return ReturnType{};
		}

		template<typename ReturnType> requires std::is_default_constructible_v<ReturnType>
		[[nodiscard]] ReturnType run(const Type::Token& label, std::string_view src, Context& context) const
		{
			static const auto compiler = Compiler::Compiler<Address>{};
			context.compilerOutput = compiler.compile(src);
			if (context.compilerOutput.functions.find(label) == context.compilerOutput.functions.end())
				throw std::runtime_error{"Undefined label " + std::string{label.name}};
			context.registerVariables.eip = reinterpret_cast<Address>(context.compilerOutput.functions.at(label).entryPoint);
			return run<ReturnType>(context);
		}

	private:
		Language::VirtualMachine::VirtualMachine<Instruction, Context> m_virtualMachine;
	};
}
