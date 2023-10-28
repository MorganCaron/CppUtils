#pragma once

#include <bit>
#include <span>
#include <array>
#include <stack>
#include <vector>
#include <cstdint>
#include <utility>
#include <variant>
#include <concepts>
#include <stdexcept>
#include <functional>

#include <CppUtils/Type/Functional.hpp>
#include <CppUtils/Type/VariadicTemplate.hpp>

namespace CppUtils::Language::VirtualMachine
{
	struct Stack final
	{
		std::vector<std::byte> data = {};
		std::vector<std::size_t> types = {};
	};

	namespace
	{
		template<Type::Concept::TriviallyCopyable T>
		[[nodiscard]] constexpr auto get(Stack& stack, std::size_t offset = 0u) -> T // C++23: 0u -> 0z
		{
			if (std::size(stack.data) < sizeof(T) + offset)
				throw std::logic_error{"Stack underflow"}; // C++23: return std::unexpected{...};
			auto buffer = std::array<std::byte, sizeof(T)>{};
			for (auto i = 0u; i < sizeof(T); ++i) // C++23: 0u -> 0z
				buffer[i] = stack.data[std::size(stack.data) - sizeof(T) - offset + i];
			return std::bit_cast<T>(buffer);
		}

		template<Type::Concept::TriviallyCopyable T>
		constexpr auto set(Stack& stack, T newValue, std::size_t offset = 0u) -> void // C++23: 0u -> 0z
		{
			if (std::size(stack.data) < sizeof(T) + offset)
				throw std::logic_error{"Stack underflow"}; // C++23: return std::unexpected{...};
			auto buffer = std::bit_cast<std::array<std::byte, sizeof(T)>>(newValue);
			for (auto i = 0u; i < sizeof(T); ++i) // C++23: 0u -> 0z
				stack.data[std::size(stack.data) - sizeof(T) - offset + i] = buffer[i];
		}

		template<Type::Concept::TriviallyCopyable T, Type::Concept::TriviallyCopyable... SupportedTypes>
		requires Type::Concept::Present<T, SupportedTypes...>
		constexpr auto push(Stack& stack, T value) -> void
		{
			stack.data.resize(std::size(stack.data) + sizeof(T), std::byte{0});
			set(stack, std::move(value));
			stack.types.push_back(Type::getPosition<T, SupportedTypes...>());
		}

		constexpr auto drop(Stack& stack, std::size_t size) -> void
		{
			if (std::size(stack.data) < size)
				throw std::logic_error{"Stack underflow"}; // C++23: return std::unexpected{...};
			stack.data.resize(std::size(stack.data) - size);
			stack.types.pop_back();
		}

		template<Type::Concept::TriviallyCopyable... Args>
		constexpr auto drop(Stack& stack) -> void
		{
			constexpr auto size = (sizeof(Args) + ...);
			static_assert(size > 0);
			drop(stack, size);
		}

		template<Type::Concept::TriviallyCopyable T>
		[[nodiscard]] constexpr auto pop(Stack& stack) -> decltype(auto)
		{
			if (std::size(stack.data) < sizeof(T))
				throw std::logic_error{"Stack underflow"}; // C++23: return std::unexpected{...};
			auto value = get<T>(stack);
			drop<T>(stack);
			return value;
		}

		template<Type::Concept::TriviallyCopyable T>
		constexpr auto copy(Stack& stack, std::size_t sourcePosition, std::size_t destinationPosition) -> void
		{
			auto sourceValue = get<T>(stack, sourcePosition);
			set(stack, sourceValue, destinationPosition);
		}

		template<Type::Concept::TriviallyCopyable... Args>
		constexpr auto paddingAfter = (0u + ... + sizeof(Args)); // C++23: 0u -> 0z

		template<std::size_t N, Type::Concept::TriviallyCopyable T, Type::Concept::TriviallyCopyable... Args>
		[[nodiscard]] constexpr auto getPaddingAfterT() -> std::size_t
		{
			if constexpr (N == 0)
			{
				if constexpr (sizeof...(Args) > 0)
					return (sizeof(Args) + ...);
				else
					return 0;
			}
			else
				return getPaddingAfterT<N - 1, Args...>();
		}

		template<class ReturnType, class... Args, std::size_t... I>
		[[nodiscard]] constexpr auto call(Stack& stack, ReturnType(*function)(Args...), [[maybe_unused]] std::index_sequence<I...> indexSequence) -> decltype(auto)
		{
			[[maybe_unused]] auto arguments = std::make_tuple(get<std::remove_reference_t<Type::NthType<I, Args...>>>(stack, getPaddingAfterT<I, std::remove_reference_t<Args>...>())...);
			return std::invoke(function, std::forward<Args>(std::get<I>(arguments))...);
		}

		template<class ReturnType, class... Args>
		constexpr auto call(Stack& stack, ReturnType(*function)(Args...)) -> void
		{
			if constexpr (std::is_void_v<ReturnType>)
				call(stack, function, std::index_sequence_for<Args...>{});
			else
				set(stack, call(stack, function, std::index_sequence_for<Args...>{}), (0 + ... + sizeof(std::remove_reference_t<Args>)));
			if constexpr (sizeof...(Args) > 0)
				stack.data.resize(std::size(stack.data) - (sizeof(std::remove_reference_t<Args>) + ...));
		}

		template<class ReturnType, class Object, class... Args, std::size_t... I>
		[[nodiscard]] constexpr auto call(Stack& stack, ReturnType(Object::*function)(Args...), Object* object, [[maybe_unused]] std::index_sequence<I...> indexSequence) -> decltype(auto)
		{
			[[maybe_unused]] auto arguments = std::make_tuple(get<std::remove_reference_t<Type::NthType<I, Args...>>>(stack, getPaddingAfterT<I, std::remove_reference_t<Args>...>())...);
			return std::invoke(function, object, std::forward<Args>(std::get<I>(arguments))...);
		}

		template<class ReturnType, class Object, class... Args>
		constexpr auto call(Stack& stack, ReturnType(Object::*function)(Args...)) -> void
		{
			auto* objectPointer = get<Object*>(stack, getPaddingAfterT<0, Object*, std::remove_reference_t<Args>...>());
			if constexpr (std::is_void_v<ReturnType>)
				call(stack, function, objectPointer, std::index_sequence_for<Args...>{});
			else
				set(stack, call(stack, function, objectPointer, std::index_sequence_for<Args...>{}), (0 + ... + sizeof(std::remove_reference_t<Args>)));
			if constexpr (sizeof...(Args) > 0)
				stack.data.resize(std::size(stack.data) - (sizeof(std::remove_reference_t<Args>) + ...));
		}

		template<class ReturnType, class Object, class... Args, std::size_t... I>
		[[nodiscard]] constexpr auto call(Stack& stack, ReturnType(Object::*function)(Args...) const, const Object* object, [[maybe_unused]] std::index_sequence<I...> indexSequence) -> decltype(auto)
		{
			[[maybe_unused]] auto arguments = std::make_tuple(get<std::remove_reference_t<Type::NthType<I, Args...>>>(stack, getPaddingAfterT<I, std::remove_reference_t<Args>...>())...);
			return std::invoke(function, object, std::forward<Args>(std::get<I>(arguments))...);
		}

		template<class ReturnType, class Object, class... Args>
		constexpr auto call(Stack& stack, ReturnType(Object::*function)(Args...) const) -> void
		{
			const auto* objectPointer = get<const Object*>(stack, getPaddingAfterT<0, const Object*, std::remove_reference_t<Args>...>());
			if constexpr (std::is_void_v<ReturnType>)
				call(stack, function, objectPointer, std::index_sequence_for<Args...>{});
			else
				set(stack, call(stack, function, objectPointer, std::index_sequence_for<Args...>{}), (0 + ... + sizeof(std::remove_reference_t<Args>)));
			if constexpr (sizeof...(Args) > 0)
				stack.data.resize(std::size(stack.data) - (sizeof(std::remove_reference_t<Args>) + ...));
		}
	}

	// Todo: Ajouter la gestion des casts
	// Todo: Ajouter une stack pour les types
	template<Type::Concept::TriviallyCopyable ReturnType, Type::Concept::TriviallyCopyable... SupportedTypes>
	constexpr auto execute(const auto& source, auto... data) -> ReturnType
	{
		auto externalData = std::array<Type::UniqueVariant<decltype(&source), decltype(data)...>, 1 + sizeof...(data)>{ &source, data... };
		[[maybe_unused]] static constinit auto typesSize = std::array<std::size_t, 1 + sizeof...(SupportedTypes)>{ sizeof(ReturnType), sizeof(SupportedTypes)... };
		static constinit auto pushTypes = std::array<void(*)(Stack&), 1 + sizeof...(SupportedTypes)>{
			+[](Stack& stack) -> void { push<ReturnType, ReturnType, SupportedTypes...>(stack, ReturnType{}); },
			+[](Stack& stack) -> void { push<SupportedTypes, ReturnType, SupportedTypes...>(stack, SupportedTypes{}); }...
		};
		static constinit auto getTypeOffset = [](Stack& stack, std::size_t position) -> std::size_t {
			auto offset = std::size_t{}; // C++23: std::size_t{} -> 0z
			while (position > 0)
				offset += typesSize[stack.types[std::size(stack.types) - position--]];
			return offset;
		};
		static constinit auto conditionalJumpTypes = std::array<void(*)(Stack&, std::size_t&, std::size_t), 1 + sizeof...(SupportedTypes)>{
			+[](Stack& stack, std::size_t& instructionPointer, std::size_t jumpDistance) -> void {
				if constexpr (std::is_constructible_v<bool, ReturnType>) if (!get<ReturnType>(stack)) instructionPointer += jumpDistance; },
			+[](Stack& stack, std::size_t& instructionPointer, std::size_t jumpDistance) -> void {
				if constexpr (std::is_constructible_v<bool, SupportedTypes>) if (!get<SupportedTypes>(stack)) instructionPointer += jumpDistance; }...
		};
		[[maybe_unused]] static constinit auto copyTypes = std::array<void(*)(Stack&, std::size_t, std::size_t), 1 + sizeof...(SupportedTypes)>{
			+[](Stack& stack, std::size_t sourcePosition, std::size_t destinationPosition) -> void { copy<ReturnType>(stack, sourcePosition, destinationPosition); },
			+[](Stack& stack, std::size_t sourcePosition, std::size_t destinationPosition) -> void { copy<SupportedTypes>(stack, sourcePosition, destinationPosition); }...
		};
		static constinit auto executeInstructionFunction = []<class ValueType>(Stack& stack, decltype(source) source, decltype(externalData) externalData, std::size_t& instructionPointer) static -> void {
			switch (auto instruction = source[instructionPointer]; instruction)
			{
			case ',': [[fallthrough]];
			case '(':
				if constexpr(Type::Concept::Present<std::size_t, ReturnType, SupportedTypes...>)
					push<std::size_t, ReturnType, SupportedTypes...>(stack, std::size_t{});
				else
					throw std::invalid_argument{"Type std::size_t missing in template parameters"}; // C++23: return std::unexpected{...};
				break;
			case ')': drop<ValueType>(stack); break;
			case '_': set(stack, ValueType{}); break;
			case 'C':
			{
				auto destinationPosition = pop<std::size_t>(stack);
				auto sourcePosition = pop<std::size_t>(stack);
				auto sourceType = stack.types[std::size(stack.types) - 1 - sourcePosition];
				auto destinationType = stack.types[std::size(stack.types) - 1 - destinationPosition];
				if (sourceType != destinationType)
					throw std::invalid_argument{"Type mismatch in copy instruction"}; // C++23: return std::unexpected{...};
				copyTypes[sourceType](stack, getTypeOffset(stack, sourcePosition), getTypeOffset(stack, destinationPosition));
				break;
			}
			// Todo: case 'D': set(stack, &get<ValueType>(stack)); break;
			case 'R': if constexpr (std::is_pointer_v<ValueType> && Type::Concept::TriviallyCopyable<std::remove_pointer_t<ValueType>>) set(stack, *get<ValueType>(stack)); break;
			case 'J': instructionPointer = pop<std::size_t>(stack); break;
			case 'P':
				if constexpr(Type::Concept::Present<std::size_t, ReturnType, SupportedTypes...>)
					push<std::size_t, ReturnType, SupportedTypes...>(stack, instructionPointer);
				else
					throw std::invalid_argument{"Type std::size_t missing in template parameters"}; // C++23: return std::unexpected{...};
				break;
			case ':':
			{
				auto id = pop<std::size_t>(stack);
				if (id >= std::size(pushTypes))
					throw std::invalid_argument{"Type id " + std::to_string(id) + " out of range"}; // C++23: return std::unexpected{...};
				pushTypes.at(id)(stack);
				break;
			}
			case ';':
			{
				auto dataId = pop<std::size_t>(stack);
				auto& data = externalData[dataId];
				std::visit([&stack, dataId](auto&& data) -> void {
					using T = std::remove_reference_t<decltype(data)>;
					if constexpr (Type::Concept::isFunctionPointer<T> || std::is_member_function_pointer_v<T>)
						call(stack, data);
					else if constexpr (!Type::Concept::Present<T, ReturnType, SupportedTypes...>)
						throw std::invalid_argument{"Type " + std::to_string(dataId) + " missing in template parameters"}; // C++23: return std::unexpected{...};
					else
						push<T, ReturnType, SupportedTypes...>(stack, data);
				}, data);
				break;
			}
			case '?':
			{
				auto jump = pop<std::size_t>(stack);
				conditionalJumpTypes[stack.types.back()](stack, instructionPointer, jump);
				break;
			}
			case '!':
				if constexpr (Type::Concept::Present<bool, ReturnType, SupportedTypes...>)
				{
					if constexpr (std::is_constructible_v<bool, ValueType>)
						push<bool, ReturnType, SupportedTypes...>(stack, !static_cast<bool>(pop<ValueType>(stack)));
					else
						drop<ValueType>(stack); // C++23: return std::unexpected{...};
				}
				else
					throw std::invalid_argument{"Type bool missing in template parameters"}; // C++23: return std::unexpected{...};
				break;
			case '=':
				if constexpr (Type::Concept::Present<bool, ReturnType, SupportedTypes...>)
				{
					[[maybe_unused]] auto rhs = pop<ValueType>(stack);
					if constexpr (std::equality_comparable<ValueType>)
						push<bool, ReturnType, SupportedTypes...>(stack, static_cast<bool>(pop<ValueType>(stack) == rhs));
				}
				else
					throw std::invalid_argument{"Type bool missing in template parameters"}; // C++23: return std::unexpected{...};
				break;
			case '<':
				if constexpr (Type::Concept::Present<bool, ReturnType, SupportedTypes...>)
				{
					[[maybe_unused]] auto rhs = pop<ValueType>(stack);
					if constexpr (std::totally_ordered<ValueType>)
						push<bool, ReturnType, SupportedTypes...>(stack, static_cast<bool>(pop<ValueType>(stack) < rhs));
				}
				else
					throw std::invalid_argument{"Type bool missing in template parameters"}; // C++23: return std::unexpected{...};
				break;
			case '>':
				if constexpr (Type::Concept::Present<bool, ReturnType, SupportedTypes...>)
				{
					[[maybe_unused]] auto rhs = pop<ValueType>(stack);
					if constexpr (std::totally_ordered<ValueType>)
						push<bool, ReturnType, SupportedTypes...>(stack, static_cast<bool>(pop<ValueType>(stack) > rhs));
				}
				else
					throw std::invalid_argument{"Type bool missing in template parameters"}; // C++23: return std::unexpected{...};
				break;
			case '&': { [[maybe_unused]] auto rhs = pop<ValueType>(stack); if constexpr (std::is_arithmetic_v<ValueType>) push<ValueType, ReturnType, SupportedTypes...>(stack, static_cast<ValueType>(pop<ValueType>(stack) & rhs)); break; }
			case '|': { [[maybe_unused]] auto rhs = pop<ValueType>(stack); if constexpr (std::is_arithmetic_v<ValueType>) push<ValueType, ReturnType, SupportedTypes...>(stack, static_cast<ValueType>(pop<ValueType>(stack) | rhs)); break; }
			case '^': { [[maybe_unused]] auto rhs = pop<ValueType>(stack); if constexpr (std::is_arithmetic_v<ValueType>) push<ValueType, ReturnType, SupportedTypes...>(stack, static_cast<ValueType>(pop<ValueType>(stack) ^ rhs)); break; }
			case '+': { [[maybe_unused]] auto rhs = pop<ValueType>(stack); if constexpr (std::is_arithmetic_v<ValueType>) push<ValueType, ReturnType, SupportedTypes...>(stack, pop<ValueType>(stack) + rhs); break; }
			case '-': { [[maybe_unused]] auto rhs = pop<ValueType>(stack); if constexpr (std::is_arithmetic_v<ValueType>) push<ValueType, ReturnType, SupportedTypes...>(stack, pop<ValueType>(stack) - rhs); break; }
			case '*': { [[maybe_unused]] auto rhs = pop<ValueType>(stack); if constexpr (std::is_arithmetic_v<ValueType>) push<ValueType, ReturnType, SupportedTypes...>(stack, pop<ValueType>(stack) * rhs); break; }
			case '/': { [[maybe_unused]] auto rhs = pop<ValueType>(stack); if constexpr (std::is_arithmetic_v<ValueType>) push<ValueType, ReturnType, SupportedTypes...>(stack, pop<ValueType>(stack) / rhs); break; }
			case '\\': if constexpr (std::is_constructible_v<ValueType, decltype(source[0])>) set(stack, static_cast<ValueType>(get<ValueType>(stack) + static_cast<ValueType>(source[instructionPointer + 1]))); break;
			case 'X': instructionPointer = std::size(source) - 1; break;
			default: if constexpr (std::is_arithmetic_v<ValueType>) if (auto c = instruction; c >= '0' && c <= '9') set(stack, static_cast<ValueType>(get<ValueType>(stack) * 10 + c - '0')); break;
			}
		};
		static constinit auto executeInstruction = std::array<void(*)(Stack&, decltype(source), decltype(externalData), std::size_t&), 1 + sizeof...(SupportedTypes)>{
			executeInstructionFunction.template operator()<ReturnType>,
			executeInstructionFunction.template operator()<SupportedTypes>...
		};
		auto stack = Stack{};
		push<ReturnType, ReturnType, SupportedTypes...>(stack, ReturnType{});
		for (auto instructionPointer = std::size_t{0}; instructionPointer < std::size(source); ++instructionPointer) // C++23: std::size_t{0} -> 0z
		{
			if (std::empty(stack.types))
				throw std::logic_error{"Stack empty"}; // C++23: return std::unexpected{...};
			executeInstruction[stack.types.back()](stack, source, externalData, instructionPointer);
		}
		return get<ReturnType>(stack);
	}
}
