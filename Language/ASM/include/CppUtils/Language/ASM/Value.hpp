#pragma once

#include <string>
#include <vector>
#include <variant>

namespace CppUtils::Language::ASM
{
	struct Value final
	{
		using Type = std::variant<char, std::size_t, int, double, std::string, std::vector<Value>>;

		Type data;
	};
}
