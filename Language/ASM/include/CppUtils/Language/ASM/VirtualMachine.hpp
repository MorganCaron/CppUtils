#pragma once

#include <stack>
#include <type_traits>
#include <functional>

#include <CppUtils/Language/ASM/Operations.hpp>
#include <CppUtils/Language/Interpreter/VirtualMachine.hpp>

namespace CppUtils::Language::ASM
{
	using namespace Type::Literals;
	using Operation = std::function<bool(Parser::Cursor<Instruction>&, Context&)>;

	template<typename Address>
	class VirtualMachine final: public Interpreter::VirtualMachine<Instruction, Context>
	{
	public:
		VirtualMachine(): Interpreter::VirtualMachine<Instruction, Context>{{
				{ "HALT"_token, Operations::halt },
				{ "NOP"_token, Operations::nop },
				{ "JUMP"_token, Operations::jump },
				{ "PUSH"_token, Operations::push },
				{ "POP"_token, Operations::pop }
			}}
		{}
	};
}
