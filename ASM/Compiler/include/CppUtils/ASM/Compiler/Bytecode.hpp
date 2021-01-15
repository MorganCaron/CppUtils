#pragma once

#include <vector>
#include <variant>
#include <optional>

#include <CppUtils/Type/Token.hpp>

namespace CppUtils::ASM::Compiler::Bytecode
{
	template<typename... Types>
	struct Instruction final
	{
		Type::Token type;
		std::optional<std::variant<Types...>> literalValue = std::nullopt;
		std::vector<std::variant<Type::Token, Types...>> parameters = {};
		std::size_t nextInstruction = 0;
		std::size_t targetInstruction = 0;
	};
}
