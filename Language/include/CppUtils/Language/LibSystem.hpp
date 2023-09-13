#pragma once

#include <CppUtils/Language/Ast.hpp>

namespace CppUtils::Language::LibSystem
{
	namespace Implementation
	{
		[[nodiscard]] inline auto allocateMemory(std::size_t size) -> void*
		{
			return new std::byte[size];
		}

		inline auto freeMemory(void* pointer) -> void
		{
			delete[] reinterpret_cast<std::byte*>(pointer);
		}
	}

	inline auto allocateMemory(Ast& context) -> void
	{
		using namespace Hash::Literals;
		auto size = static_cast<std::size_t>(context.getStackNode(0).value);
		auto* address = Implementation::allocateMemory(size);
		Ast::getFirstNode(context.root["register"_token][0]).value = reinterpret_cast<Hash::Token>(address);
	}

	inline auto freeMemory(Ast& context) -> void
	{
		auto* address = reinterpret_cast<void*>(context.getStackNode(0).value);
		Implementation::freeMemory(address);
	}
}
