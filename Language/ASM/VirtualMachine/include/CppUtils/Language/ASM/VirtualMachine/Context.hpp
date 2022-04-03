#pragma once

#include <stack>
#include <unordered_map>

namespace CppUtils::Language::ASM::VirtualMachine
{
	struct Context final
	{
		std::unordered_map<std::size_t, std::uintptr_t> registerFile;
		std::stack<std::uintptr_t> stack;
	};
}
