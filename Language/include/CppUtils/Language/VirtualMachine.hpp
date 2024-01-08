#pragma once

#include <span>
#include <array>
#include <stack>
#include <cstdint>
#include <utility>
#include <variant>
#include <concepts>
#include <stdexcept>
#include <functional>

#include <CppUtils/Log/Logger.hpp>
#include <CppUtils/String/String.hpp>
#include <CppUtils/Container/Stack.hpp>

// Ribosome

namespace CppUtils::Language::VirtualMachine
{
	namespace
	{
		template<class Stack, class ReturnType, class... Args, std::size_t... I>
		[[nodiscard]] constexpr auto call(Stack& stack, ReturnType(*function)(Args...), [[maybe_unused]] std::index_sequence<I...> indexSequence) -> decltype(auto)
		{
			return std::invoke(function, stack.template get<std::remove_cvref_t<Type::NthType<I, Args...>>>(std::size(stack) - sizeof...(Args) + I)...);
		}

		template<class Stack, class ReturnType, class... Args>
		constexpr auto call(Stack& stack, ReturnType(*function)(Args...)) -> void
		{
			if constexpr (std::is_void_v<ReturnType>)
				call(stack, function, std::index_sequence_for<Args...>{});
			else
				stack.set(std::size(stack) - 1 - sizeof...(Args), call(stack, function, std::index_sequence_for<Args...>{}));
			for (auto i = 0uz; i < sizeof...(Args); ++i)
				stack.drop();
		}

		template<class Stack, class ReturnType, class Object, class... Args, std::size_t... I>
		[[nodiscard]] constexpr auto call(Stack& stack, ReturnType(Object::*function)(Args...), Object* object, [[maybe_unused]] std::index_sequence<I...> indexSequence) -> decltype(auto)
		{
			return std::invoke(function, object, stack.template get<std::remove_cvref_t<Type::NthType<I, Args...>>>(std::size(stack) - sizeof...(Args) + I)...);
		}

		template<class Stack, class ReturnType, class Object, class... Args>
		constexpr auto call(Stack& stack, ReturnType(Object::*function)(Args...)) -> void
		{
			auto* objectPointer = stack.template get<Object*>(std::size(stack) - 1 - sizeof...(Args));
			if constexpr (std::is_void_v<ReturnType>)
				call(stack, function, objectPointer, std::index_sequence_for<Args...>{});
			else
				stack.set(std::size(stack) - 2 - sizeof...(Args), call(stack, function, objectPointer, std::index_sequence_for<Args...>{}));
			for (auto i = 0uz; i < sizeof...(Args); ++i)
				stack.drop();
		}

		template<class Stack, class ReturnType, class Object, class... Args, std::size_t... I>
		[[nodiscard]] constexpr auto call(Stack& stack, ReturnType(Object::*function)(Args...) const, const Object* object, [[maybe_unused]] std::index_sequence<I...> indexSequence) -> decltype(auto)
		{
			return std::invoke(function, object, stack.template get<std::remove_cvref_t<Type::NthType<I, Args...>>>(std::size(stack) - sizeof...(Args) + I)...);
		}

		template<class Stack, class ReturnType, class Object, class... Args>
		constexpr auto call(Stack& stack, ReturnType(Object::*function)(Args...) const) -> void
		{
			const auto* objectPointer = stack.template get<const Object*>(std::size(stack) - 1 - sizeof...(Args));
			if constexpr (std::is_void_v<ReturnType>)
				call(stack, function, objectPointer, std::index_sequence_for<Args...>{});
			else
				stack.set(std::size(stack) - 2 - sizeof...(Args), call(stack, function, objectPointer, std::index_sequence_for<Args...>{}));
			for (auto i = 0uz; i < sizeof...(Args); ++i)
				stack.drop();
		}

		[[nodiscard]] inline constexpr auto isModeAbsolute(std::size_t mode) noexcept -> bool { return mode & 0b01; }
		[[nodiscard]] inline constexpr auto isModeRelative(std::size_t mode) noexcept -> bool { return !isModeAbsolute(mode); }
		[[nodiscard]] inline constexpr auto isModeIndirect(std::size_t mode) noexcept -> bool { return mode & 0b10; }
		[[nodiscard]] inline constexpr auto isModeDirect(std::size_t mode) noexcept -> bool { return !isModeIndirect(mode); }
	}
	
	template<Type::Concept::TriviallyCopyable ReturnType, Type::Concept::TriviallyCopyable... SupportedTypes>
	requires Type::Concept::Present<std::size_t, ReturnType, SupportedTypes...>
	constexpr auto execute(const auto& source, auto... data) -> ReturnType
	{
		using Logger = Logger<"CppUtils">;
		using Stack = Container::Stack<ReturnType, SupportedTypes...>;
		auto externalData = std::array<Type::UniqueVariant<decltype(&source), decltype(data)...>, 1 + sizeof...(data)>{ &source, data... };
		static constexpr auto getPositionInSource = [](Stack& stack, std::size_t instructionPointer, std::size_t mode, std::size_t position) -> std::size_t {
			if (isModeIndirect(mode))
				position = stack.template get<std::size_t>(std::size(stack) - 1 - position);
			return isModeRelative(mode) ? (instructionPointer + position) : position;
		};
		static constexpr auto applyModeToPosition = [](Stack& stack, std::size_t mode, std::size_t position) -> std::size_t {
			return (isModeDirect(mode) || isModeRelative(mode)) ? (std::size(stack) - 1 - position) : position;
		};
		static constexpr auto getValueWithMode = [](Stack& stack, auto&& visitor) -> void {
			if (auto mode = stack.template pop<std::size_t>(); isModeDirect(mode))
				stack.visit(std::size(stack) - 1, [&stack, visitor = std::forward<decltype(visitor)>(visitor)](auto&& value) -> void {
					stack.drop();
					visitor(std::forward<decltype(value)>(value));
				});
			else
			{
				auto position = stack.template pop<std::size_t>();
				position = applyModeToPosition(stack, mode, position);
				stack.visit(position, std::forward<decltype(visitor)>(visitor));
			}
		};
		static constexpr auto printType = [](Stack& stack, std::size_t position) -> void {
			stack.visit(position, [position, type = stack.getType(position)](auto&& value) -> void {
				Logger::print<"debug">("Position: {}; Type: {}; Size: {} bytes; Value: {}", position, type, sizeof(decltype(value)), String::formatValue(value));
			});
		};
		static constexpr auto executeInstructionFunction = []<class ValueType>(Stack& stack, decltype(source) source, decltype(externalData) externalData, std::size_t& instructionPointer) static -> void {
			switch (auto instruction = source[instructionPointer]; instruction)
			{
			case ',': [[fallthrough]];
			case '(': stack.push(0uz); break;
			case ')': stack.drop(); break;
			case '_': stack.set(std::size(stack) - 1, ValueType{}); break;
			case 'C': // Todo: renommer en M ?
			{
				auto destinationMode = stack.template pop<std::size_t>();
				auto destinationPosition = stack.template pop<std::size_t>();
				auto sourceMode = stack.template pop<std::size_t>();
				auto sourcePosition = stack.template pop<std::size_t>();
				sourcePosition = applyModeToPosition(stack, sourceMode, sourcePosition);
				destinationPosition = applyModeToPosition(stack, destinationMode, destinationPosition);
				stack.copy(sourcePosition, destinationPosition);
			}
			break;
			// Todo: case 'D': stack.push(&stack.template pop<ValueType>()); break;
			case 'I':
				Logger::print<"debug">("Stack size: {} elements; {} bytes", std::size(stack), stack.getByteSize());
				for (auto i = 0uz; i < std::size(stack); ++i)
					printType(stack, i);
				break;
			case 'J':
			{
				auto mode = stack.template pop<std::size_t>();
				auto destination = stack.template pop<std::size_t>();
				instructionPointer = getPositionInSource(stack, instructionPointer, mode, destination);
			}
			break;
			case 'P': stack.template push<std::size_t>(instructionPointer); break;
			case 'R':
				if constexpr (!std::is_pointer_v<ValueType>)
					throw std::logic_error{"The dereferenced type is not a pointer"};
				else if constexpr (using DereferencedType = std::decay_t<decltype(*std::declval<ValueType>())>;
					!Type::Concept::Present<DereferencedType, ReturnType, SupportedTypes...>)
					throw std::logic_error{"The dereferenced type is not present among the supported types"};
				else
					stack.template push<DereferencedType>(*stack.template pop<ValueType>());
				break;
			case 'W': printType(stack, stack.template pop<std::size_t>()); break;
			case 'X': instructionPointer = std::size(source) - 1; break;
			case ':': stack.pushType(stack.template pop<std::size_t>()); break;
			case ';':
			{
				auto dataId = stack.template pop<std::size_t>();
				std::visit([&stack, dataId](auto&& data) -> void {
					using T = std::remove_cvref_t<decltype(data)>;
					if constexpr (Type::Concept::isFunctionPointer<T> || std::is_member_function_pointer_v<T>)
						call(stack, data);
					else if constexpr (!Type::Concept::Present<T, ReturnType, SupportedTypes...>)
						throw std::invalid_argument{"Type " + std::to_string(dataId) + " missing in template parameters"};
					else
						stack.template push<T>(data);
				}, externalData[dataId]);
			}
			break;
			case '?':
			{
				auto jumpMode = stack.template pop<std::size_t>();
				auto jumpDestination = stack.template pop<std::size_t>();
				getValueWithMode(stack, [&stack, &instructionPointer, jumpMode, jumpDestination](auto&& condition) -> void {
					if constexpr (!std::is_constructible_v<bool, decltype(condition)>)
						throw std::invalid_argument{"The type used in conditional jump instruction is not convertible to boolean"};
					else if (!condition)
						instructionPointer = getPositionInSource(stack, instructionPointer, jumpMode, jumpDestination);
				});
			}
			break;
			case '!':
				// Todo: Ajouter un bit d'adressage
				if constexpr (!Type::Concept::Present<bool, ReturnType, SupportedTypes...>)
					throw std::invalid_argument{"Type bool missing in template parameters"};
				else if constexpr (!std::is_constructible_v<bool, ValueType>)
					throw std::invalid_argument{"Type not convertible to boolean"};
				else
					stack.push(!static_cast<bool>(stack.template pop<ValueType>()));
				break;
			case '=':
				// Todo: Ajouter deux bits d'adressage
				if constexpr (!Type::Concept::Present<bool, ReturnType, SupportedTypes...>)
					throw std::invalid_argument{"Type bool missing in template parameters"};
				else if constexpr (std::equality_comparable<ValueType>)
				{
					auto rhs = stack.template pop<ValueType>();
					stack.push(static_cast<bool>(stack.template pop<ValueType>() == rhs));
				}
				break;
			case '<':
				// Todo: Ajouter deux bits d'adressage
				if constexpr (!Type::Concept::Present<bool, ReturnType, SupportedTypes...>)
					throw std::invalid_argument{"Type bool missing in template parameters"};
				else if constexpr (std::totally_ordered<ValueType>)
				{
					auto rhs = stack.template pop<ValueType>();
					stack.push(static_cast<bool>(stack.template pop<ValueType>() < rhs));
				}
				break;
			case '>':
				// Todo: Ajouter deux bits d'adressage
				if constexpr (!Type::Concept::Present<bool, ReturnType, SupportedTypes...>)
					throw std::invalid_argument{"Type bool missing in template parameters"};
				else if constexpr (std::totally_ordered<ValueType>)
				{
					auto rhs = stack.template pop<ValueType>();
					stack.push(static_cast<bool>(stack.template pop<ValueType>() > rhs));
				}
				break;
			// Todo: Ajouter deux bits d'adressage:
			case '&': if constexpr (std::is_arithmetic_v<ValueType>) { auto rhs = stack.template pop<ValueType>(); stack.push(static_cast<ValueType>(stack.template pop<ValueType>() & rhs)); } break;
			case '|': if constexpr (std::is_arithmetic_v<ValueType>) { auto rhs = stack.template pop<ValueType>(); stack.push(static_cast<ValueType>(stack.template pop<ValueType>() | rhs)); } break;
			case '^': if constexpr (std::is_arithmetic_v<ValueType>) { auto rhs = stack.template pop<ValueType>(); stack.push(static_cast<ValueType>(stack.template pop<ValueType>() ^ rhs)); } break;
			case '+': if constexpr (std::is_arithmetic_v<ValueType>) { auto rhs = stack.template pop<ValueType>(); stack.template push<ValueType>(stack.template pop<ValueType>() + rhs); } break;
			case '-': if constexpr (std::is_arithmetic_v<ValueType>) { auto rhs = stack.template pop<ValueType>(); stack.template push<ValueType>(stack.template pop<ValueType>() - rhs); } break;
			case '*': if constexpr (std::is_arithmetic_v<ValueType>) { auto rhs = stack.template pop<ValueType>(); stack.template push<ValueType>(stack.template pop<ValueType>() * rhs); } break;
			case '/': if constexpr (std::is_arithmetic_v<ValueType>) { auto rhs = stack.template pop<ValueType>(); stack.template push<ValueType>(stack.template pop<ValueType>() / rhs); } break;
			case '\\': if constexpr (std::is_constructible_v<ValueType, decltype(source[0])>) stack.set(std::size(stack) - 1, static_cast<ValueType>(stack.template top<ValueType>() + static_cast<ValueType>(source[++instructionPointer]))); break;
			default: if constexpr (std::is_arithmetic_v<ValueType>) if (auto c = instruction; c >= '0' && c <= '9') stack.set(std::size(stack) - 1, static_cast<ValueType>(stack.template top<ValueType>() * 10 + static_cast<ValueType>(c - '0'))); break;
			}
		};
		static constexpr auto executeInstruction = std::array<void(*)(Stack&, decltype(source), decltype(externalData), std::size_t&), 1 + sizeof...(SupportedTypes)>{
			executeInstructionFunction.template operator()<ReturnType>,
			executeInstructionFunction.template operator()<SupportedTypes>...
		};
		auto stack = Stack{ReturnType{}};
		for (auto instructionPointer = 0uz; instructionPointer < std::size(source); ++instructionPointer)
		{
			if (std::empty(stack)) [[unlikely]]
				throw std::logic_error{"Stack empty"};
			executeInstruction[stack.getType(std::size(stack) - 1)](stack, source, externalData, instructionPointer);
		}
		return stack.template top<ReturnType>();
	}
}
