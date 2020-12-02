#pragma once

#include <vector>

namespace CppUtils::Language::Interpreter
{
	template<typename Instruction>
	struct Cursor final
	{
		explicit Cursor(const std::vector<Instruction>& c_instructions, const std::size_t c_pos = 0):
			instructions{c_instructions},
			pos{c_pos}
		{}

		[[nodiscard]] inline const Instruction& getInstruction() const
		{
			return instructions.at(pos);
		}

		inline void exit() noexcept
		{
			pos = instructions.size();
		}

		const std::vector<Instruction>& instructions;
		std::size_t pos;
	};
}
