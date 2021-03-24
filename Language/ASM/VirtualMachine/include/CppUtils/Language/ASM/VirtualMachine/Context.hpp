#pragma once

#include <stack>
#include <unordered_map>

namespace CppUtils::Language::ASM::VirtualMachine
{
	template<typename Type>
	struct Context final
	{
		std::unordered_map<std::size_t, Type> registerFile;
		std::stack<Type> stack;
	};
}
