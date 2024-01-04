#pragma once

#include <bit>
#include <array>
#include <vector>
#include <stdexcept>

#include <CppUtils/Type/VariadicTemplate.hpp>

namespace CppUtils::Container
{
	template<Type::Concept::TriviallyCopyable... SupportedTypes>
	class Stack final
	{
	public:
		using Types = std::tuple<SupportedTypes...>;
		static constexpr auto typesSize = std::array<std::size_t, sizeof...(SupportedTypes)>{ sizeof(SupportedTypes)... };

		template<Type::Concept::TriviallyCopyable... Args>
		requires (Type::Concept::Present<Args, SupportedTypes...> && ...)
		constexpr explicit Stack(Args... values)
		{
			(push(values), ...);
		}

		[[nodiscard]] constexpr auto empty() const noexcept -> bool
		{
			return std::empty(m_types);
		}

		[[nodiscard]] constexpr auto size() const noexcept -> std::size_t
		{
			return std::size(m_types);
		}

		[[nodiscard]] constexpr auto getType(std::size_t position) const -> std::size_t
		{
			if (position >= std::size(m_types)) [[unlikely]]
				throw std::out_of_range{"Stack::get(std::size_t position) : Out of range"};
			return m_types[position];
		}

		template<Type::Concept::TriviallyCopyable T>
		requires Type::Concept::Present<T, SupportedTypes...>
		[[nodiscard]] constexpr auto get(std::size_t position) const -> T
		{
			if (Type::getPosition<T, SupportedTypes...>() != getType(position)) [[unlikely]]
				throw std::logic_error{"The type at the specified position does not match the stored type"};
			
			auto offset = getTypeOffset(position);
			auto buffer = std::array<std::byte, sizeof(T)>{};
			for (auto i = 0uz; i < sizeof(T); ++i)
				buffer[i] = m_data[offset + i];
			return std::bit_cast<T>(buffer);
		}

		template<Type::Concept::TriviallyCopyable T>
		requires Type::Concept::Present<T, SupportedTypes...>
		constexpr auto set(std::size_t position, T newValue) -> void
		{
			if (Type::getPosition<T, SupportedTypes...>() != getType(position)) [[unlikely]]
				throw std::logic_error{"The type at the specified position does not match the stored type"};
			
			auto offset = getTypeOffset(position);
			auto buffer = std::bit_cast<std::array<std::byte, sizeof(T)>>(newValue);
			for (auto i = 0uz; i < sizeof(T); ++i)
				m_data[offset + i] = buffer[i];
		}

		template<Type::Concept::TriviallyCopyable T>
		requires Type::Concept::Present<T, SupportedTypes...>
		constexpr auto push(T value) -> void
		{
			m_types.push_back(Type::getPosition<T, SupportedTypes...>());
			m_data.resize(std::size(m_data) + sizeof(T), std::byte{0});
			set(size() - 1, std::move(value));
		}

		template<Type::Concept::TriviallyCopyable T>
		requires Type::Concept::Present<T, SupportedTypes...>
		constexpr auto drop() -> void
		{
			if (empty()) [[unlikely]]
				throw std::underflow_error{"Stack::drop() : Container already empty"};
			if (Type::getPosition<T, SupportedTypes...>() != m_types.back()) [[unlikely]]
				throw std::logic_error{"The type at the specified position does not match the stored type"};
			m_data.resize(std::size(m_data) - sizeof(T));
			m_types.pop_back();
		}

		template<Type::Concept::TriviallyCopyable T>
		requires Type::Concept::Present<T, SupportedTypes...>
		[[nodiscard]] constexpr auto pop() -> decltype(auto)
		{
			if (empty()) [[unlikely]]
				throw std::underflow_error{"Stack::drop() : Container already empty"};
			if (Type::getPosition<T, SupportedTypes...>() != m_types.back()) [[unlikely]]
				throw std::logic_error{"The type at the specified position does not match the stored type"};
			auto value = get<T>(0);
			drop<T>();
			return value;
		}

		template<Type::Concept::TriviallyCopyable SourceType, Type::Concept::TriviallyCopyable DestinationType = SourceType>
		constexpr auto copy(std::size_t sourcePosition, std::size_t destinationPosition) -> void
		{
			set(destinationPosition, static_cast<DestinationType>(get<SourceType>(sourcePosition)));
		}

	private:
		[[nodiscard]] constexpr auto getTypeOffset(std::size_t position) const -> std::size_t
		{
			if (position >= std::size(m_types))
				throw std::out_of_range{"Stack: Out of range"};
			auto offset = 0uz;
			for (auto i = 0uz; i < position; ++i)
				offset += typesSize[m_types[i]];
			return offset;
		};

		std::vector<std::byte> m_data = {};
		std::vector<std::size_t> m_types = {};
	};
}
