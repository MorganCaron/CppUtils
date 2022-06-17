#pragma once

#include <array>
#include <cstring>
#include <concepts>
#include <stdexcept>

namespace CppUtils::Language::VirtualMachine
{
	template<std::size_t stackSize>
	class Stack final
	{
	public:
		Stack(): m_top{0}, m_stack{}
		{}

		template<std::default_initializable Type>
		Type& push(Type value)
		{
			if (m_top + sizeof(Type) >= stackSize)
				throw std::runtime_error{"Stack overflow"};
			std::memcpy(&m_stack.data()[m_top], &value, sizeof(Type));
			m_top += sizeof(Type);
			return top<Type>();
		}

		template<std::default_initializable Type>
		Type pop()
		{
			if (m_top < sizeof(Type))
				throw std::runtime_error{"Stack underflow"};
			auto value = Type{};
			std::memcpy(&value, &m_stack.data()[m_top - sizeof(Type)], sizeof(Type));
			m_top -= sizeof(Type);
			return std::move(value);
		}

		template<class Type>
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
