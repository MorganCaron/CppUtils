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

#include <CppUtils/Log/Logger.hpp>
#include <CppUtils/String/String.hpp>
#include <CppUtils/Type/VariadicTemplate.hpp>

// Ribosome

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
		[[nodiscard]] constexpr auto get(Stack& stack, std::size_t offset = 0) -> T
		{
			if (std::size(stack.data) < sizeof(T) + offset) [[unlikely]]
				throw std::logic_error{"Stack underflow"};
			auto buffer = std::array<std::byte, sizeof(T)>{};
			for (auto i = 0uz; i < sizeof(T); ++i)
				buffer[i] = stack.data[std::size(stack.data) - sizeof(T) - offset + i];
			return std::bit_cast<T>(buffer);
		}

		template<Type::Concept::TriviallyCopyable T>
		constexpr auto set(Stack& stack, T newValue, std::size_t offset = 0) -> void
		{
			if (std::size(stack.data) < sizeof(T) + offset) [[unlikely]]
				throw std::logic_error{"Stack underflow"};
			auto buffer = std::bit_cast<std::array<std::byte, sizeof(T)>>(newValue);
			for (auto i = 0uz; i < sizeof(T); ++i)
				stack.data[std::size(stack.data) - sizeof(T) - offset + i] = buffer[i];
		}

		template<Type::Concept::TriviallyCopyable T, Type::Concept::TriviallyCopyable... SupportedTypes>
		requires Type::Concept::Present<T, SupportedTypes...>
		constexpr auto push(Stack& stack, T value) -> void
		{
			stack.types.push_back(Type::getPosition<T, SupportedTypes...>());
			stack.data.resize(std::size(stack.data) + sizeof(T), std::byte{0});
			set(stack, std::move(value));
		}

		template<Type::Concept::TriviallyCopyable T>
		constexpr auto drop(Stack& stack) -> void
		{
			if (std::size(stack.data) < sizeof(T)) [[unlikely]]
				throw std::logic_error{"Stack underflow"};
			stack.data.resize(std::size(stack.data) - sizeof(T));
			stack.types.pop_back();
		}

		template<Type::Concept::TriviallyCopyable T>
		[[nodiscard]] constexpr auto pop(Stack& stack) -> decltype(auto)
		{
			if (std::size(stack.data) < sizeof(T)) [[unlikely]]
				throw std::logic_error{"Stack underflow"};
			auto value = get<T>(stack);
			drop<T>(stack);
			return value;
		}

		template<Type::Concept::TriviallyCopyable SourceType, Type::Concept::TriviallyCopyable DestinationType = SourceType>
		constexpr auto copy(Stack& stack, std::size_t sourcePosition, std::size_t destinationPosition) -> void
		{
			set(stack, static_cast<DestinationType>(get<SourceType>(stack, sourcePosition)), destinationPosition);
		}

		template<Type::Concept::TriviallyCopyable... Args>
		constexpr auto paddingAfter = (0uz + ... + sizeof(Args));

		template<std::size_t N, Type::Concept::TriviallyCopyable T, Type::Concept::TriviallyCopyable... Args>
		[[nodiscard]] constexpr auto getPaddingAfterT() noexcept -> std::size_t
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
			(..., drop<std::remove_reference_t<Args>>(stack));
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
				set(stack, call(stack, function, objectPointer, std::index_sequence_for<Args...>{}), sizeof(objectPointer) + (0 + ... + sizeof(std::remove_reference_t<Args>)));
			(..., drop<std::remove_reference_t<Args>>(stack));
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
				set(stack, call(stack, function, objectPointer, std::index_sequence_for<Args...>{}), sizeof(objectPointer) + (0 + ... + sizeof(std::remove_reference_t<Args>)));
			(..., drop<std::remove_reference_t<Args>>(stack));
		}

		template<class T>
		constexpr auto conditionalJump(Stack& stack, std::size_t& instructionPointer, std::size_t jumpDistance) -> void
		{
			if constexpr (std::is_constructible_v<bool, T>)
				if (!pop<T>(stack))
					instructionPointer += jumpDistance;
		}
	}
	
	template<Type::Concept::TriviallyCopyable ReturnType, Type::Concept::TriviallyCopyable... SupportedTypes>
	constexpr auto execute(const auto& source, auto... data) -> ReturnType
	{
		using Logger = Logger<"CppUtils">;
		auto externalData = std::array<Type::UniqueVariant<decltype(&source), decltype(data)...>, 1 + sizeof...(data)>{ &source, data... };
		static constexpr auto typesSize = std::array<std::size_t, 1 + sizeof...(SupportedTypes)>{ sizeof(ReturnType), sizeof(SupportedTypes)... };
		static constexpr auto pushTypes = std::array<void(*)(Stack&), 1 + sizeof...(SupportedTypes)>{
			+[](Stack& stack) -> void { push<ReturnType, ReturnType, SupportedTypes...>(stack, ReturnType{}); },
			+[](Stack& stack) -> void { push<SupportedTypes, ReturnType, SupportedTypes...>(stack, SupportedTypes{}); }...
		};
		static constexpr auto getTypeOffset = [](Stack& stack, std::size_t position) -> std::size_t {
			auto offset = 0uz;
			while (position > 0)
				offset += typesSize[stack.types[std::size(stack.types) - position--]];
			return offset;
		};
		static constexpr auto conditionalJumpTypes = std::array<void(*)(Stack&, std::size_t&, std::size_t), 1 + sizeof...(SupportedTypes)>{
			&conditionalJump<ReturnType>, &conditionalJump<SupportedTypes>...
		};
		static constexpr auto copyTypesBuilder = []<class SourceType>() consteval -> auto {
			return std::array<void(*)(Stack&, std::size_t, std::size_t), 1 + sizeof...(SupportedTypes)>{
				+[](Stack& stack, std::size_t sourcePosition, std::size_t destinationPosition) -> void { if constexpr (std::is_convertible_v<SourceType, ReturnType>) return copy<SourceType, ReturnType>(stack, sourcePosition, destinationPosition); else return {}; },
				+[](Stack& stack, std::size_t sourcePosition, std::size_t destinationPosition) -> void { if constexpr (std::is_convertible_v<SourceType, SupportedTypes>) return copy<SourceType, SupportedTypes>(stack, sourcePosition, destinationPosition); else return {}; }...
			};
		};
		static constexpr auto copyTypes = std::array<std::array<void(*)(Stack&, std::size_t, std::size_t), 1 + sizeof...(SupportedTypes)>, 1 + sizeof...(SupportedTypes)>{
			copyTypesBuilder.template operator()<ReturnType>(),
			copyTypesBuilder.template operator()<SupportedTypes>()...
		};
		static constexpr auto printType = []<class ValueType>(Stack& stack, std::size_t position) static -> void {
			auto type = stack.types[std::size(stack.types) - 1 - position];
			auto value = get<ValueType>(stack, getTypeOffset(stack, position));
			Logger::print<"debug">("Position: {}; Type: {}; Size: {} bytes; Value: {}", position, type, sizeof(ValueType), String::formatValue(value));
		};
		static constexpr auto printTypes = std::array<void(*)(Stack&, std::size_t), 1 + sizeof...(SupportedTypes)>{
			printType.template operator()<ReturnType>,
			printType.template operator()<SupportedTypes>...
		};
		static constexpr auto executeInstructionFunction = []<class ValueType>(Stack& stack, decltype(source) source, decltype(externalData) externalData, std::size_t& instructionPointer) static -> void {
			switch (auto instruction = source[instructionPointer]; instruction)
			{
			case ',': [[fallthrough]];
			case '(':
				if constexpr (Type::Concept::Present<std::size_t, ReturnType, SupportedTypes...>)
					push<std::size_t, ReturnType, SupportedTypes...>(stack, 0uz);
				else
					throw std::invalid_argument{"Type std::size_t missing in template parameters"};
				break;
			case ')': drop<ValueType>(stack); break;
			case '_': set(stack, ValueType{}); break;
			case 'C':
			{
				auto destinationPosition = pop<std::size_t>(stack);
				auto sourcePosition = pop<std::size_t>(stack);
				auto sourceType = stack.types[std::size(stack.types) - 1 - sourcePosition];
				auto destinationType = stack.types[std::size(stack.types) - 1 - destinationPosition];
				copyTypes[sourceType][destinationType](stack, getTypeOffset(stack, sourcePosition), getTypeOffset(stack, destinationPosition));
				break;
			}
			// Todo: case 'D': set(stack, &get<ValueType>(stack)); break;
			case 'R':
				if constexpr (std::is_pointer_v<ValueType> && Type::Concept::TriviallyCopyable<std::remove_pointer_t<ValueType>>)
					set(stack, *get<ValueType>(stack));
				break;
			case 'I':
				Logger::print<"debug">("Stack size: {} elements; {} bytes", std::size(stack.types), std::size(stack.data));
				for (auto i = 0uz; i < std::size(stack.types); ++i)
					printTypes[stack.types[std::size(stack.types) - 1 - i]](stack, i);
				break;
			case 'J': instructionPointer = get<std::size_t>(stack); break;
			case 'P':
				if constexpr (Type::Concept::Present<std::size_t, ReturnType, SupportedTypes...>)
					push<std::size_t, ReturnType, SupportedTypes...>(stack, instructionPointer);
				else
					throw std::invalid_argument{"Type std::size_t missing in template parameters"};
				break;
			case 'W':
			{
				auto id = pop<std::size_t>(stack);
				if (id >= std::size(pushTypes)) [[unlikely]]
					throw std::invalid_argument{"Type id " + std::to_string(id) + " out of range"};
				printTypes[stack.types[std::size(stack.types) - 1 - id]](stack, id);
				break;
			}
			case 'X': instructionPointer = std::size(source) - 1; break;
			case ':':
			{
				auto id = pop<std::size_t>(stack);
				if (id >= std::size(pushTypes)) [[unlikely]]
					throw std::invalid_argument{"Type id " + std::to_string(id) + " out of range"};
				pushTypes[id](stack);
				break;
			}
			case ';':
			{
				auto dataId = pop<std::size_t>(stack);
				std::visit([&stack, dataId](auto&& data) -> void {
					using T = std::remove_reference_t<decltype(data)>;
					if constexpr (Type::Concept::isFunctionPointer<T> || std::is_member_function_pointer_v<T>)
						call(stack, data);
					else if constexpr (!Type::Concept::Present<T, ReturnType, SupportedTypes...>)
						throw std::invalid_argument{"Type " + std::to_string(dataId) + " missing in template parameters"};
					else
						push<T, ReturnType, SupportedTypes...>(stack, data);
				}, externalData[dataId]);
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
						drop<ValueType>(stack);
				}
				else
					throw std::invalid_argument{"Type bool missing in template parameters"};
				break;
			case '=':
				if constexpr (!Type::Concept::Present<bool, ReturnType, SupportedTypes...>)
					throw std::invalid_argument{"Type bool missing in template parameters"};
				else if constexpr (std::equality_comparable<ValueType>)
				{
					auto rhs = pop<ValueType>(stack);
					push<bool, ReturnType, SupportedTypes...>(stack, static_cast<bool>(pop<ValueType>(stack) == rhs));
				}
				break;
			case '<':
				if constexpr (!Type::Concept::Present<bool, ReturnType, SupportedTypes...>)
					throw std::invalid_argument{"Type bool missing in template parameters"};
				else if constexpr (std::totally_ordered<ValueType>)
				{
					auto rhs = pop<ValueType>(stack);
					push<bool, ReturnType, SupportedTypes...>(stack, static_cast<bool>(pop<ValueType>(stack) < rhs));
				}
				break;
			case '>':
				if constexpr (!Type::Concept::Present<bool, ReturnType, SupportedTypes...>)
					throw std::invalid_argument{"Type bool missing in template parameters"};
				else if constexpr (std::totally_ordered<ValueType>)
				{
					auto rhs = pop<ValueType>(stack);
					push<bool, ReturnType, SupportedTypes...>(stack, static_cast<bool>(pop<ValueType>(stack) > rhs));
				}
				break;
			case '&': if constexpr (std::is_arithmetic_v<ValueType>) { auto rhs = pop<ValueType>(stack); push<ValueType, ReturnType, SupportedTypes...>(stack, static_cast<ValueType>(pop<ValueType>(stack) & rhs)); } break;
			case '|': if constexpr (std::is_arithmetic_v<ValueType>) { auto rhs = pop<ValueType>(stack); push<ValueType, ReturnType, SupportedTypes...>(stack, static_cast<ValueType>(pop<ValueType>(stack) | rhs)); } break;
			case '^': if constexpr (std::is_arithmetic_v<ValueType>) { auto rhs = pop<ValueType>(stack); push<ValueType, ReturnType, SupportedTypes...>(stack, static_cast<ValueType>(pop<ValueType>(stack) ^ rhs)); } break;
			case '+': if constexpr (std::is_arithmetic_v<ValueType>) { auto rhs = pop<ValueType>(stack); push<ValueType, ReturnType, SupportedTypes...>(stack, pop<ValueType>(stack) + rhs); } break;
			case '-': if constexpr (std::is_arithmetic_v<ValueType>) { auto rhs = pop<ValueType>(stack); push<ValueType, ReturnType, SupportedTypes...>(stack, pop<ValueType>(stack) - rhs); } break;
			case '*': if constexpr (std::is_arithmetic_v<ValueType>) { auto rhs = pop<ValueType>(stack); push<ValueType, ReturnType, SupportedTypes...>(stack, pop<ValueType>(stack) * rhs); } break;
			case '/': if constexpr (std::is_arithmetic_v<ValueType>) { auto rhs = pop<ValueType>(stack); push<ValueType, ReturnType, SupportedTypes...>(stack, pop<ValueType>(stack) / rhs); } break;
			case '\\': if constexpr (std::is_constructible_v<ValueType, decltype(source[0])>) set(stack, static_cast<ValueType>(get<ValueType>(stack) + static_cast<ValueType>(source[++instructionPointer]))); break;
			default: if constexpr (std::is_arithmetic_v<ValueType>) if (auto c = instruction; c >= '0' && c <= '9') set(stack, static_cast<ValueType>(get<ValueType>(stack) * 10 + static_cast<ValueType>(c - '0'))); break;
			}
		};
		static constexpr auto executeInstruction = std::array<void(*)(Stack&, decltype(source), decltype(externalData), std::size_t&), 1 + sizeof...(SupportedTypes)>{
			executeInstructionFunction.template operator()<ReturnType>,
			executeInstructionFunction.template operator()<SupportedTypes>...
		};
		auto stack = Stack{};
		push<ReturnType, ReturnType, SupportedTypes...>(stack, ReturnType{});
		for (auto instructionPointer = 0uz; instructionPointer < std::size(source); ++instructionPointer)
		{
			if (std::empty(stack.types)) [[unlikely]]
				throw std::logic_error{"Stack empty"};
			executeInstruction[stack.types.back()](stack, source, externalData, instructionPointer);
		}
		return get<ReturnType>(stack);
	}
}
