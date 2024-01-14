#pragma once

#include <bit>
#include <array>
#include <format>
#include <vector>
#include <stdexcept>

#include <CppUtils/Log/Logger.hpp>
#include <CppUtils/String/String.hpp>
#include <CppUtils/Type/VariadicTemplate.hpp>

namespace CppUtils::Container
{
	template<Type::Concept::TriviallyCopyable... SupportedTypes>
	class Stack final
	{
	public:
		using Logger = Logger<"CppUtils">;
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

		[[nodiscard]] constexpr auto getByteSize() const noexcept -> std::size_t
		{
			return std::size(m_data);
		}

		[[nodiscard]] constexpr auto getType(std::size_t position) const -> std::size_t
		{
			if (position >= size()) [[unlikely]]
				throw std::out_of_range{std::format("Stack::getType({}) : Out of range", position)};
			return m_types[position];
		}

		template<Type::Concept::TriviallyCopyable T>
		requires Type::Concept::Present<T, SupportedTypes...>
		[[nodiscard]] constexpr auto get(std::size_t position) const -> T
		{
			if (position >= size()) [[unlikely]]
				throw std::out_of_range{std::format("Stack::get({}) : Out of range", position)};
			if (auto storedType = m_types[position]; Type::getPosition<T, SupportedTypes...>() != storedType) [[unlikely]]
				throw std::logic_error{std::format("Stack::get({}) : The type at the specified position does not match the stored type (Type: {})", position, storedType)};
			
			auto offset = getTypeOffset(position);
			auto buffer = std::array<std::byte, sizeof(T)>{};
			for (auto i = 0uz; i < sizeof(T); ++i)
				buffer[i] = m_data[offset + i];
			return std::bit_cast<T>(buffer);
		}

		template<Type::Concept::TriviallyCopyable T>
		requires Type::Concept::Present<T, SupportedTypes...>
		[[nodiscard]] constexpr auto top() const -> decltype(auto)
		{
			return get<T>(size() - 1);
		}

		template<Type::Concept::TriviallyCopyable T>
		requires Type::Concept::Present<T, SupportedTypes...>
		constexpr auto set(std::size_t position, T newValue) -> void
		{
			if (position >= size()) [[unlikely]]
				throw std::out_of_range{std::format("Stack::set({}, T newValue) : Out of range", position)};
			if (auto storedType = m_types[position]; Type::getPosition<T, SupportedTypes...>() != storedType) [[unlikely]]
				throw std::logic_error{std::format("Stack::set(std::size_t position, T newValue) : The type at the specified position does not match the stored type (Type: {})", storedType)};
			
			auto offset = getTypeOffset(position);
			auto buffer = std::bit_cast<std::array<std::byte, sizeof(T)>>(newValue);
			for (auto i = 0uz; i < sizeof(T); ++i)
				m_data[offset + i] = buffer[i];
		}

		template<Type::Concept::TriviallyCopyable T>
		requires Type::Concept::Present<T, SupportedTypes...>
		constexpr auto set(T newValue) -> void
		{
			set(size() - 1, std::move(newValue));
		}

		template<Type::Concept::TriviallyCopyable T>
		requires Type::Concept::Present<T, SupportedTypes...>
		constexpr auto push(T value = T{}) -> void
		{
			m_types.push_back(Type::getPosition<T, SupportedTypes...>());
			m_data.resize(getByteSize() + sizeof(T), std::byte{0});
			set(size() - 1, std::move(value));
		}

		constexpr auto pushType(std::size_t type) -> void
		{
			if (type >= sizeof...(SupportedTypes))
				throw std::out_of_range{std::format("Stack::pushType({}) : Out of range", type)};
			static constexpr auto pushbyType = std::array{
				+[](Stack<SupportedTypes...>& stack) -> void {
					stack.template push<SupportedTypes>();
				}...
			};
			pushbyType[type](*this);
		}

		constexpr auto drop() -> void
		{
			static constexpr auto dropFunctions = std::array{
				+[](const Stack<SupportedTypes...>& stack, std::vector<std::byte>& data) -> void {
					data.resize(stack.getByteSize() - sizeof(SupportedTypes));
				}...
			};

			if (empty()) [[unlikely]]
				throw std::underflow_error{"Stack::drop() : Container already empty"};

			dropFunctions[m_types[size() - 1]](*this, m_data);
			m_types.pop_back();
		}

		template<Type::Concept::TriviallyCopyable T>
		requires Type::Concept::Present<T, SupportedTypes...>
		[[nodiscard]] constexpr auto pop() -> decltype(auto)
		{
			if (empty()) [[unlikely]]
				throw std::underflow_error{"Stack::pop() : Container already empty"};
			if (auto storedType = m_types.back(); Type::getPosition<T, SupportedTypes...>() != storedType) [[unlikely]]
				throw std::logic_error{std::format("Stack::pop() : The type at the specified position does not match the stored type (Type: {})", storedType)};
			auto value = get<T>(size() - 1);
			drop();
			return value;
		}

		constexpr auto pop(auto&& visitor) -> void
		{
			visit(size() - 1, [this, visitor = std::forward<decltype(visitor)>(visitor)](auto&& value) mutable -> void {
				drop();
				visitor(std::forward<decltype(value)>(value));
			});
		}

		constexpr auto copy(std::size_t sourcePosition, std::size_t destinationPosition) -> void
		{
			static constexpr auto copyToDestination = []<class SourceType>() consteval -> auto {
				return std::array<void(*)(Stack<SupportedTypes...>&, std::size_t, std::size_t), sizeof...(SupportedTypes)>{
					+[](Stack<SupportedTypes...>& stack, std::size_t sourcePosition, std::size_t destinationPosition) -> void {
						if constexpr (!std::is_convertible_v<SourceType, SupportedTypes>)
							throw std::logic_error{std::format("Stack::copy({}, {}) : The source type is not convertible to the destination type", sourcePosition, destinationPosition)};
						else
							stack.set(destinationPosition, static_cast<SupportedTypes>(stack.template get<SourceType>(sourcePosition)));
					}...
				};
			};
			static constexpr auto copyFromSource = std::array<std::array<void(*)(Stack<SupportedTypes...>&, std::size_t, std::size_t), sizeof...(SupportedTypes)>, sizeof...(SupportedTypes)>{
				copyToDestination.template operator()<SupportedTypes>()...
			};

			if (sourcePosition >= size())
				throw std::out_of_range{std::format("Stack::copy({}, {}) : Source out of range", sourcePosition, destinationPosition)};
			if (destinationPosition >= size())
				throw std::out_of_range{std::format("Stack::copy({}, {}) : Destination out of range", sourcePosition, destinationPosition)};

			copyFromSource[m_types[sourcePosition]][m_types[destinationPosition]](*this, sourcePosition, destinationPosition);
		}

		constexpr auto visit(std::size_t position, auto&& visitor) -> void
		{
			static constexpr auto visitors = std::array{
				+[](const Stack<SupportedTypes...>& stack, std::size_t position, decltype(visitor) visitor) -> void {
					visitor(stack.template get<SupportedTypes>(position));
				}...
			};

			if (position >= size())
				throw std::out_of_range{std::format("Stack::visit({}, auto&& visitor) : Out of range", position)};

			visitors[m_types[position]](*this, position, std::forward<decltype(visitor)>(visitor));
		}

		constexpr auto print(std::size_t position) -> void
		{
			visit(position, [position, type = getType(position)](auto&& value) -> void {
				Logger::print<"debug">("Position: {}; Type: {}; Size: {} bytes; Value: {}", position, type, sizeof(decltype(value)), String::formatValue(value));
			});
		};

		constexpr auto print() -> void
		{
			Logger::print<"debug">("Stack size: {} elements; {} bytes", size(), getByteSize());
			for (auto i = size(); i > 0;)
				print(--i);
		};

	private:
		[[nodiscard]] constexpr auto getTypeOffset(std::size_t position) const -> std::size_t
		{
			auto offset = 0uz;
			for (auto i = 0uz; i < position; ++i)
				offset += typesSize[m_types[i]];
			return offset;
		}

		std::vector<std::byte> m_data = {};
		std::vector<std::size_t> m_types = {};
	};
}
