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

		inline void run(std::span<const std::unique_ptr<Instruction>> instructions, Context<Type> context = {}) const
		{
			m_virtualMachine.run(instructions, context);
		}

		static void run(std::string_view src)
		{
			const auto bytecode = Compiler::compile<Type>(src);
			static const auto vm = VM::VirtualMachine<Type>{};
			vm.run(bytecode);
		}

	private:
		Language::VM::VirtualMachine<Instruction, Context<Type>> m_virtualMachine;
	};
}
