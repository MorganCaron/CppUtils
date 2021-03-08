#pragma once

#include <memory>
#include <vector>

namespace CppUtils::Language::Compiler
{
	template<typename Instruction>
	struct Context
	{
		std::vector<std::unique_ptr<Instruction>> instructions;

		template<typename... Args>
		inline Instruction* createInstruction(Args... args)
		{
			return instructions.emplace_back(std::make_unique<Instruction>(std::forward<Args>(args)...)).get();
		}
	};
}
