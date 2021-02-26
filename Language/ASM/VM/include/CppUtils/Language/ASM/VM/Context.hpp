#pragma once

#include <stack>
#include <variant>
#include <unordered_map>

#include <CppUtils/Language/ASM/Compiler/Bytecode.hpp>

namespace CppUtils::Language::ASM::VM
{
	template<typename... Types>
	struct Context final
	{
		std::unordered_map<Type::Token, std::variant<Types...>, Type::Token::hash_fn> registerFile;
		std::stack<std::variant<Types...>> stack;
	};
}
