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
	
	class VirtualMachine final
	{
	public:
		using Instruction = Compiler::Bytecode::Instruction;

		VirtualMachine(): m_virtualMachine{{
				{ "nop"_token, Operations::nop },
				{ "halt"_token, Operations::halt },
				{ "move"_token, Operations::move },
				{ "add"_token, Operations::add }/*,
				{ "jump"_token, Operations::jump },
				{ "push"_token, Operations::push },
				{ "pop"_token, Operations::pop }*/
			}}
		{}

		void run(std::span<const std::unique_ptr<Instruction>> instructions, Context& context) const
		{
			if (instructions.empty())
				return;
			auto* instruction = instructions[0].get();
			m_virtualMachine.run(instruction->type, instruction, context);
		}

		void run(std::string_view src, Context& context) const
		{
			static const auto compiler = Compiler::Compiler{};
			run(compiler.compile(src).instructions, context);
		}

	private:
		Language::VirtualMachine::VirtualMachine<Instruction*, Context> m_virtualMachine;
	};
}
