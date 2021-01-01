#pragma once

#include <CppUtils/Type/Token.hpp>
#include <CppUtils/Language/Interpreter/Cursor.hpp>

namespace CppUtils::Language::ASM
{
	using namespace Type::Literals;

	struct Instruction final
	{
		Type::Token type;
		std::vector<Type::Token> parameters = {};
		Instruction* nextInstruction = nullptr;
		Instruction* targetInstruction = nullptr;
	};

	using Cursor = Interpreter::Cursor<Instruction>;
}
