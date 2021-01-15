#pragma once

#include <CppUtils/Language/ASM/Instruction.hpp>
#include <CppUtils/Language/ASM/Context.hpp>

namespace CppUtils::Language::ASM::Operations
{
	inline void halt(Parser::Cursor<Instruction>& cursor, [[maybe_unused]] Context& context) noexcept
	{
		cursor.position = cursor.end();
	}

	inline void nop(Parser::Cursor<Instruction>& cursor, [[maybe_unused]] Context& context) noexcept
	{
		++cursor.position;
	}

	inline void jump(Parser::Cursor<Instruction>& cursor, [[maybe_unused]] Context& context)
	{
		[[maybe_unused]] const auto& parameters = cursor.getElement().parameters;

		// cursor.position = parameters.at(0).id;
	}

	inline void move(Parser::Cursor<Instruction>& cursor, Context& context)
	{
		const auto& parameters = cursor.getElement().parameters;

		context.registerFile[parameters.at(0)] = context.registerFile.at(parameters.at(1));
		++cursor.position;
	}
	
	inline void push(Parser::Cursor<Instruction>& cursor, Context& context)
	{
		const auto& parameters = cursor.getElement().parameters;

		context.stack.emplace(context.registerFile.at(parameters.at(0)));
		++cursor.position;
	}

	inline void pop(Parser::Cursor<Instruction>& cursor, Context& context)
	{
		const auto& parameters = cursor.getElement().parameters;

		if (context.stack.size() == 0)
			throw std::runtime_error{"Stack underflow"};

		context.registerFile[parameters.at(0)] = context.stack.top();
		context.stack.pop();
		++cursor.position;
	}
}
