#pragma once

#include <memory>
#include <vector>
#include <utility>
#include <functional>

namespace CppUtils::Language::Compiler
{
	template<typename Instruction>
	struct Output
	{
		std::vector<std::unique_ptr<Instruction>> instructions = {};
	};

	template<typename Compiler, typename Instruction, typename CompilerOutput>
	requires std::derived_from<CompilerOutput, Output<Instruction>>
	struct Context
	{
		std::reference_wrapper<const Compiler> compiler;
		CompilerOutput output;

		explicit Context(std::reference_wrapper<const Compiler> c_compiler): compiler{c_compiler}
		{}

		template<typename... Args>
		inline Instruction* createInstruction(Args... args)
		{
			return output.instructions.emplace_back(std::make_unique<Instruction>(std::forward<Args>(args)...)).get();
		}
	};
}
