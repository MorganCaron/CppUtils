#pragma once

#include <CppUtils/Type/TypeId.hpp>
#include <CppUtils/Language/Interpreter/Cursor.hpp>

namespace CppUtils::Language::ASM
{
	using namespace Type::Literals;

	struct Instruction final
	{
		Type::TypeId type;
		std::vector<Type::TypeId> parameters = {};
		Instruction* nextInstruction = nullptr;
		Instruction* targetInstruction = nullptr;
	};

	using Cursor = Interpreter::Cursor<Instruction>;
}
