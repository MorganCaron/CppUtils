#pragma once

#include <CppUtils/Language/ASM/Instruction.hpp>
#include <CppUtils/Language/ASM/Context.hpp>

namespace CppUtils::Language::ASM::Operations
{
	inline void halt(Cursor& cursor, [[maybe_unused]] Context& context) noexcept
	{
		cursor.exit();
	}

	inline void nop(Cursor& cursor, [[maybe_unused]] Context& context) noexcept
	{
		++cursor.pos;
	}

	inline void jump(Cursor& cursor, [[maybe_unused]] Context& context)
	{
		const auto& parameters = cursor.getElement().parameters;

		cursor.pos = parameters.at(0).id;
	}

	inline void move(Cursor& cursor, Context& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		auto& [registerFile, stack] = context;

		registerFile[parameters.at(0)] = registerFile.at(parameters.at(1));
		++cursor.pos;
	}
	
	inline void push(Cursor& cursor, Context& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		auto& [registerFile, stack] = context;

		stack.emplace(registerFile.at(parameters.at(0)));
		++cursor.pos;
	}

	inline void pop(Cursor& cursor, Context& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		auto& [registerFile, stack] = context;

		if (stack.size() == 0)
			throw std::runtime_error{"Stack underflow"};

		registerFile[parameters.at(0)] = stack.top();
		stack.pop();
		++cursor.pos;
	}
}
