#pragma once

#include <stack>
#include <type_traits>
#include <functional>

#include <CppUtils/Language/ASM/Operations.hpp>
#include <CppUtils/Language/Interpreter/VirtualMachine.hpp>

namespace CppUtils::Language::ASM
{
	using namespace Type::Literals;

	using Operation = std::function<void(Cursor&, Context&)>;

	template<typename Address>
	class VirtualMachine final: public Interpreter::VirtualMachine<Instruction, Context>
	{
	public:
		VirtualMachine(): Interpreter::VirtualMachine<Instruction, Context>{{
				{ "HALT"_typeId, Operations::halt },
				{ "NOP"_typeId, Operations::nop },
				{ "JUMP"_typeId, Operations::jump },
				{ "PUSH"_typeId, Operations::push },
				{ "POP"_typeId, Operations::pop }
			}}
		{}
	};
}
