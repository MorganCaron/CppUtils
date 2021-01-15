#pragma once

#include <CppUtils/Type/Token.hpp>

namespace CppUtils::Language::ASM
{
	using namespace Type::Literals;

	struct Instruction final
	{
		Type::Token type;
		std::vector<Type::Token> parameters = {};
		std::size_t nextInstruction = 0;
		std::size_t targetInstruction = 0;
	};

	using Iterator = typename std::span<const Instruction>::iterator&;
}
