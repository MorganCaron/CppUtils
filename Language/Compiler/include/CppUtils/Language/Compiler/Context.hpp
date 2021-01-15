#pragma once

namespace CppUtils::Language::Compiler
{
	template<typename IRInstruction, typename BytecodeInstruction>
	struct Context final
	{
		std::reference_wrapper<IRInstruction> irInstruction;
		std::reference_wrapper<std::vector<BytecodeInstruction>> bytecode;
	};
}
