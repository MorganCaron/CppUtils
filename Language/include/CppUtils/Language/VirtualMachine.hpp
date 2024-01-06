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
			(..., stack.template drop<std::remove_cvref_t<Args>>());
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
			(..., stack.template drop<std::remove_cvref_t<Args>>());
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
			(..., stack.template drop<std::remove_cvref_t<Args>>());
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
			return (isModeDirect(mode) || isModeRelative(mode)) ? position : (std::size(stack) - 1 - position);
		};
		static constexpr auto getValueOnStack = []<class ValueType>(Stack& stack) -> ValueType {
			if (auto mode = stack.template pop<std::size_t>(); isModeDirect(mode))
				return stack.template pop<ValueType>();
			else
			{
				auto valuePosition = stack.template pop<std::size_t>();
				valuePosition = applyModeToPosition(stack, mode, valuePosition);
				return stack.template get<ValueType>(std::size(stack) - 1 - valuePosition);
			}
		};
		static constexpr auto conditionalJump = []<class ValueType>(Stack& stack, std::size_t& instructionPointer) static -> void {
			if constexpr (!std::is_constructible_v<bool, ValueType>)
				throw std::invalid_argument{"The type used in conditional jump is not convertible to boolean"};
			else
			{
				auto jumpMode = stack.template pop<std::size_t>();
				auto jumpDestination = stack.template pop<std::size_t>();
				if (auto conditionValue = getValueOnStack.template operator()<ValueType>(stack); !conditionValue)
					instructionPointer = getPositionInSource(stack, instructionPointer, jumpMode, jumpDestination);
			}
		};
		static constexpr auto conditionalJumpTypes = std::array<void(*)(Stack&, std::size_t&), 1 + sizeof...(SupportedTypes)>{
			conditionalJump.template operator()<ReturnType>,
			conditionalJump.template operator()<SupportedTypes>...
		};
		static constexpr auto copyTypesBuilder = []<class SourceType>() consteval -> auto {
			return std::array<void(*)(Stack&, std::size_t, std::size_t), 1 + sizeof...(SupportedTypes)>{
				+[](Stack& stack, std::size_t sourcePosition, std::size_t destinationPosition) -> void { if constexpr (std::is_convertible_v<SourceType, ReturnType>) return stack.template copy<SourceType, ReturnType>(sourcePosition, destinationPosition); else return {}; },
				+[](Stack& stack, std::size_t sourcePosition, std::size_t destinationPosition) -> void { if constexpr (std::is_convertible_v<SourceType, SupportedTypes>) return stack.template copy<SourceType, SupportedTypes>(sourcePosition, destinationPosition); else return {}; }...
			};
		};
		static constexpr auto copyTypes = std::array<std::array<void(*)(Stack&, std::size_t, std::size_t), 1 + sizeof...(SupportedTypes)>, 1 + sizeof...(SupportedTypes)>{
			copyTypesBuilder.template operator()<ReturnType>(),
			copyTypesBuilder.template operator()<SupportedTypes>()...
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
			case ')': stack.template drop<ValueType>(); break;
			case '_': stack.set(std::size(stack) - 1, ValueType{}); break;
			case 'C': // Todo: renommer en M ?
			{
				// Todo: Ajouter deux bits d'adressage
				auto destinationPosition = stack.template pop<std::size_t>();
				auto sourcePosition = stack.template pop<std::size_t>();
				auto sourceType = stack.getType(std::size(stack) - 1 - sourcePosition);
				auto destinationType = stack.getType(std::size(stack) - 1 - destinationPosition);
				copyTypes[sourceType][destinationType](stack, std::size(stack) - 1 - sourcePosition, std::size(stack) - 1 - destinationPosition);
				break;
			}
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
				break;
			}
			case '?':
			{
				if (std::size(stack) <= 4)
					throw std::underflow_error{"Stack Underflow: Not enough parameters passed to conditional jump"};
				else
					conditionalJumpTypes[stack.getType(std::size(stack) - 4)](stack, instructionPointer);
				break;
			}
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
