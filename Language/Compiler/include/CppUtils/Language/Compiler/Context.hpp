#pragma once

#include <memory>
#include <vector>
#include <utility>
#include <functional>

namespace CppUtils::Language::Compiler
{
	template<typename Compiler, typename Instruction>
	struct Context
	{
		std::reference_wrapper<const Compiler> compiler;
		std::vector<std::unique_ptr<Instruction>> instructions = {};

		explicit Context(std::reference_wrapper<const Compiler> c_compiler): compiler{c_compiler}
		{}

		template<typename... Args>
		inline Instruction* createInstruction(Args... args)
		{
			return instructions.emplace_back(std::make_unique<Instruction>(std::forward<Args>(args)...)).get();
		}
	};
}
