#pragma once

#include <stack>
#include <type_traits>
#include <functional>

#include <CppUtils/Language/ASM/Operations.hpp>
#include <CppUtils/Language/Interpreter/VM.hpp>

namespace CppUtils::Language::ASM
{
	using namespace std::literals;
	using namespace Type::Literals;

	using Operation = std::function<void(Cursor&, RegisterFile&, std::stack<Value>&)>;

	class VM final
	{
	public:
		VM():
			m_vm{{
				{ "HALT"_typeId, Operations::halt },
				{ "NOP"_typeId, Operations::nop },
				{ "JUMP"_typeId, Operations::jump },
				{ "PUSH"_typeId, Operations::push },
				{ "POP"_typeId, Operations::pop }
			}}
		{}

		inline void setInstructions(std::vector<Instruction> instructions) noexcept
		{
			m_vm.setInstructions(std::move(instructions));
		}

		inline void run(Context& context) const
		{
			m_vm.run(context);
		}

	private:
		Interpreter::VM<Instruction, Context> m_vm;
	};
}
