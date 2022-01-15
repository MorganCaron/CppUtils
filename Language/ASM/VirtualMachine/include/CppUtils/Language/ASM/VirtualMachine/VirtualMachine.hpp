#pragma once

#include <stack>
#include <type_traits>
#include <functional>

#include <CppUtils/Language/ASM/Compiler/Compiler.hpp>
#include <CppUtils/Language/ASM/VirtualMachine/Context.hpp>
#include <CppUtils/Language/ASM/VirtualMachine/Operations.hpp>
#include <CppUtils/Language/VirtualMachine/VirtualMachine.hpp>

namespace CppUtils::Language::ASM::VirtualMachine
{
	using namespace Type::Literals;

	template<typename Address>
	class VirtualMachine final
	{
	public:
		using Instruction = Compiler::Bytecode::Instruction<Address>;

		VirtualMachine(): m_virtualMachine{{
				{ "move"_token, Operations<Instruction, Address>::move },
				{ "add"_token, Operations<Instruction, Address>::add }/*,
				{ "jump"_token, Operations<Types...>::jump },
				{ "push"_token, Operations<Types...>::push },
				{ "pop"_token, Operations<Types...>::pop }*/
			}}
		{}

		void run(std::span<const std::unique_ptr<Instruction>> instructions, Context<Address>& context) const
		{
			m_virtualMachine.run(instructions, context);
		}

		void run(std::string_view src, Context<Address>& context) const
		{
			static const auto compiler = Compiler::Compiler<Address>{};
			run(compiler.compile(src).instructions, context);
		}

	private:
		Language::VirtualMachine::VirtualMachine<Instruction, Context<Address>> m_virtualMachine;
	};
}
