#pragma once

#include <array>
#include <cstring>
#include <stdexcept>
#include <type_traits>

namespace CppUtils::Language::VirtualMachine
{
	template<std::size_t stackSize>
	class Stack final
	{
	public:
		Stack(): m_top{0}, m_stack{}
		{}

		template<typename Type> requires std::is_default_constructible_v<Type>
		Type& push(Type value)
		{
			if (m_top + sizeof(Type) >= stackSize)
				throw std::runtime_error{"Stack overflow"};
			std::memcpy(&m_stack.data()[m_top], &value, sizeof(Type));
			m_top += sizeof(Type);
			return top<Type>();
		}

		template<typename Type> requires std::is_default_constructible_v<Type>
		Type pop()
		{
			if (m_top < sizeof(Type))
				throw std::runtime_error{"Stack underflow"};
			auto value = Type{};
			std::memcpy(&value, &m_stack.data()[m_top - sizeof(Type)], sizeof(Type));
			m_top -= sizeof(Type);
			return std::move(value);
		}

		template<typename Type>
		[[nodiscard]] Type& top()
		{
			if (m_top == 0)
				throw std::runtime_error{"Empty stack"};
			if (m_top < sizeof(Type))
				throw std::runtime_error{"Reading of a too big type"};
			return *reinterpret_cast<Type*>(m_stack.data() + m_top - sizeof(Type));
		}

		[[nodiscard]] constexpr std::size_t size() const noexcept
		{
			return stackSize;
		}

		[[nodiscard]] constexpr bool empty() const noexcept
		{
			return size() == 0;
		}

	private:
		std::size_t m_top;
		std::array<std::byte, stackSize> m_stack;
	};
}
