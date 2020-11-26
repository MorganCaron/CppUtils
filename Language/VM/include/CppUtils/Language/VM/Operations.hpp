#pragma once

#include <stack>

#include <CppUtils/Language/VM/Cursor.hpp>

namespace CppUtils::Language::VM::Operations
{
	inline void nop(Cursor& cursor, [[maybe_unused]] std::stack<Bytecode::Instruction>& stack) noexcept
	{
		++cursor.pos;
	}

	inline void push(Cursor& cursor, std::stack<Bytecode::Instruction>& stack)
	{
		stack.emplace(cursor.getInstruction());
		++cursor.pos;
	}
}
