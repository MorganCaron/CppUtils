#pragma once

#include <CppUtils/Language/ASM/Instruction.hpp>
#include <CppUtils/Language/ASM/Context.hpp>

namespace CppUtils::Language::ASM::Operations
{
	inline void halt([[maybe_unused]] Iterator instructionIterator, Context& context) noexcept
	{
		context.running = false;
	}

	inline void nop(Iterator instructionIterator, [[maybe_unused]] Context& context) noexcept
	{
		++instructionIterator;
	}

	inline void jump(Iterator instructionIterator, [[maybe_unused]] Context& context)
	{
		[[maybe_unused]] const auto& parameters = instructionIterator->parameters;

		// instructionIterator.pos = parameters.at(0).id;
	}

	inline void move(Iterator instructionIterator, Context& context)
	{
		const auto& parameters = instructionIterator->parameters;

		context.registerFile[parameters.at(0)] = context.registerFile.at(parameters.at(1));
		++instructionIterator;
	}
	
	inline void push(Iterator instructionIterator, Context& context)
	{
		const auto& parameters = instructionIterator->parameters;

		context.stack.emplace(context.registerFile.at(parameters.at(0)));
		++instructionIterator;
	}

	inline void pop(Iterator instructionIterator, Context& context)
	{
		const auto& parameters = instructionIterator->parameters;

		if (context.stack.size() == 0)
			throw std::runtime_error{"Stack underflow"};

		context.registerFile[parameters.at(0)] = context.stack.top();
		context.stack.pop();
		++instructionIterator;
	}
}
