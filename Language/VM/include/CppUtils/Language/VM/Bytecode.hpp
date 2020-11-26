#pragma once

namespace CppUtils::Language::VM::Bytecode
{
	enum class InstructionType: std::size_t
	{
		HLT = 0,
		NOP
	};

	struct Instruction final
	{
		InstructionType type;
	};
}
