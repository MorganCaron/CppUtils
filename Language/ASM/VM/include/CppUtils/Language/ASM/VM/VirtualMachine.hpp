#pragma once

#include <stack>
#include <type_traits>
#include <functional>

#include <CppUtils/Language/ASM/Compiler/Compiler.hpp>
#include <CppUtils/Language/ASM/VM/Context.hpp>
#include <CppUtils/Language/ASM/VM/Operations.hpp>
#include <CppUtils/Language/VM/VirtualMachine.hpp>

namespace CppUtils::Language::ASM::VM
{
	using namespace Type::Literals;

	template<typename Type>
	class VirtualMachine final
	{
	public:
		using Instruction = Compiler::Bytecode::Instruction<Type>;

		VirtualMachine(): m_virtualMachine{{
				{ "move"_token, Operations<Instruction, Type>::move },
				{ "add"_token, Operations<Instruction, Type>::add }/*,
				{ "jump"_token, Operations<Types...>::jump },
				{ "push"_token, Operations<Types...>::push },
				{ "pop"_token, Operations<Types...>::pop }*/
			}}
		{}

		inline void run(std::span<const std::unique_ptr<Instruction>> instructions, Context<Type>& context) const
		{
			m_virtualMachine.run(instructions, context);
		}

		inline void run(std::string_view src, Context<Type>& context) const
		{
			static const auto compiler = Compiler::Compiler<Type>{};
			run(compiler.compile(src).instructions, context);
		}

	private:
		Language::VM::VirtualMachine<Instruction, Context<Type>> m_virtualMachine;
	};
}
