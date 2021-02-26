#pragma once

#include <stack>
#include <type_traits>
#include <functional>

#include <CppUtils/Language/ASM/Compiler/Bytecode.hpp>
#include <CppUtils/Language/ASM/VM/Context.hpp>
#include <CppUtils/Language/ASM/VM/Operations.hpp>
#include <CppUtils/Language/VM/VirtualMachine.hpp>

namespace CppUtils::Language::ASM::VM
{
	using namespace Type::Literals;

	template<typename... Types>
	class VirtualMachine final
	{
	public:
		using Instruction = Compiler::Bytecode::Instruction<Types...>;

		VirtualMachine(): m_virtualMachine{{
				{ "halt"_token, Operations<Instruction, Types...>::halt },
				{ "nop"_token, Operations<Instruction, Types...>::nop },
				{ "move"_token, Operations<Instruction, Types...>::move },
				{ "add"_token, Operations<Instruction, Types...>::add }/*,
				{ "jump"_token, Operations<Types...>::jump },
				{ "push"_token, Operations<Types...>::push },
				{ "pop"_token, Operations<Types...>::pop }*/
			}}
		{}

		inline void run(std::span<const Instruction> instructions) const
		{
			auto context = Context<Types...>{};
			m_virtualMachine.run(instructions, context);
		}

		static void run(std::string_view src)
		{
			const auto ast = parse<Types...>(src);
			static const auto compiler = Compiler::Compiler<Types...>{};
			const auto bytecode = compiler.compile(ast);
			static const auto vm = VM::VirtualMachine<Types...>{};
			vm.run(bytecode);
		}

	private:
		Language::VM::VirtualMachine<Instruction, Context<Types...>> m_virtualMachine;
	};
}
