#pragma once

#include <stack>
#include <unordered_map>

#include <CppUtils/Type/Token.hpp>
#include <CppUtils/Language/ASM/Value.hpp>

namespace CppUtils::Language::ASM
{
	using RegisterFile = std::unordered_map<Type::Token, Value, Type::Token::hash_fn>;

	struct Context final
	{
		RegisterFile registerFile;
		std::stack<Value> stack;
	};
}
