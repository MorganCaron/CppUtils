#pragma once

#include <vector>

#include <CppUtils/Language/VM/Bytecode.hpp>

namespace CppUtils::Language::VM
{
	struct Cursor
	{
		explicit Cursor(const std::vector<Bytecode::Instruction>& c_instructions, const std::size_t c_pos = 0):
			instructions{c_instructions},
			pos{c_pos}
		{}

		[[nodiscard]] inline const Bytecode::Instruction& getInstruction() const
		{
			return instructions.at(pos);
		}

		const std::vector<Bytecode::Instruction>& instructions;
		std::size_t pos;
	};
}
