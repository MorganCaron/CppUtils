#pragma once

#include <bit>
#include <span>
#include <array>
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
	template<Type::Concept::TriviallyCopyable T>
	[[nodiscard]] constexpr auto get(std::span<std::byte> stack, std::size_t offset = 0) -> T
	{
		if (std::size(stack) < sizeof(T) + offset)
			throw std::logic_error{"Stack underflow"};
		auto data = std::array<std::byte, sizeof(T)>{};
		for (auto i = 0u; i < sizeof(T); ++i)
			data[i] = stack[std::size(stack) - sizeof(T) - offset + i];
		return std::bit_cast<T>(data);
	}

	constexpr auto set(std::vector<std::byte>& stack, Type::Concept::TriviallyCopyable auto value, std::size_t offset = 0) -> void
	{
		if (std::size(stack) < sizeof(decltype(value)) + offset)
			throw std::logic_error{"Stack underflow"};
		auto data = std::bit_cast<std::array<std::byte, sizeof(decltype(value))>>(value);
		for (auto i = 0u; i < sizeof(decltype(value)); ++i)
			stack[std::size(stack) - sizeof(decltype(value)) - offset + i] = data[i];
	}

	constexpr auto push(std::vector<std::byte>& stack, Type::Concept::TriviallyCopyable auto value) -> void
	{
		stack.resize(std::size(stack) + sizeof(decltype(value)), std::byte{0});
		set(stack, std::move(value));
	}

	template<Type::Concept::TriviallyCopyable... Args>
	constexpr auto pop(std::vector<std::byte>& stack) -> void
	{
		if constexpr (sizeof...(Args) > 0)
		{
			if (std::size(stack) < (sizeof(Args) + ...))
				throw std::logic_error{"Stack underflow"};
			stack.resize(std::size(stack) - (sizeof(Args) + ...));
		}
	}

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
	[[nodiscard]] constexpr auto call([[maybe_unused]] std::span<std::byte> stack, ReturnType(*function)(Args...), [[maybe_unused]] std::index_sequence<I...> indexSequence) -> decltype(auto)
	{
		[[maybe_unused]] auto arguments = std::make_tuple(get<std::remove_reference_t<Type::NthType<I, Args...>>>(stack, getPaddingAfterT<I, std::remove_reference_t<Args>...>())...);
		return std::invoke(function, std::forward<Args>(std::get<I>(arguments))...);
	}

	template<class ReturnType, class... Args>
	constexpr auto call(std::vector<std::byte>& stack, ReturnType(*function)(Args...)) -> void
	{
		if constexpr (std::is_void_v<ReturnType>)
			call(stack, function, std::index_sequence_for<Args...>{});
		else
			set(stack, call(stack, function, std::index_sequence_for<Args...>{}), (0 + ... + sizeof(std::remove_reference_t<Args>)));
		if constexpr (sizeof...(Args) > 0)
			stack.resize(std::size(stack) - (sizeof(std::remove_reference_t<Args>) + ...));
	}

	template<class ReturnType, class Object, class... Args, std::size_t... I>
	[[nodiscard]] constexpr auto call([[maybe_unused]] std::span<std::byte> stack, ReturnType(Object::*function)(Args...), Object* object, [[maybe_unused]] std::index_sequence<I...> indexSequence) -> decltype(auto)
	{
		[[maybe_unused]] auto arguments = std::make_tuple(get<std::remove_reference_t<Type::NthType<I, Args...>>>(stack, getPaddingAfterT<I, std::remove_reference_t<Args>...>())...);
		return std::invoke(function, object, std::forward<Args>(std::get<I>(arguments))...);
	}

	template<class ReturnType, class Object, class... Args>
	constexpr auto call(std::vector<std::byte>& stack, ReturnType(Object::*function)(Args...)) -> void
	{
		auto* objectPointer = get<Object*>(stack, getPaddingAfterT<0, Object*, std::remove_reference_t<Args>...>());
		if constexpr (std::is_void_v<ReturnType>)
			call(stack, function, objectPointer, std::index_sequence_for<Args...>{});
		else
			set(stack, call(stack, function, objectPointer, std::index_sequence_for<Args...>{}), (0 + ... + sizeof(std::remove_reference_t<Args>)));
		if constexpr (sizeof...(Args) > 0)
			stack.resize(std::size(stack) - (sizeof(std::remove_reference_t<Args>) + ...));
	}

	template<class ReturnType, class Object, class... Args, std::size_t... I>
	[[nodiscard]] constexpr auto call([[maybe_unused]] std::span<std::byte> stack, ReturnType(Object::*function)(Args...) const, const Object* object, [[maybe_unused]] std::index_sequence<I...> indexSequence) -> decltype(auto)
	{
		[[maybe_unused]] auto arguments = std::make_tuple(get<std::remove_reference_t<Type::NthType<I, Args...>>>(stack, getPaddingAfterT<I, std::remove_reference_t<Args>...>())...);
		return std::invoke(function, object, std::forward<Args>(std::get<I>(arguments))...);
	}

	template<class ReturnType, class Object, class... Args>
	constexpr auto call(std::vector<std::byte>& stack, ReturnType(Object::*function)(Args...) const) -> void
	{
		const auto* objectPointer = get<const Object*>(stack, getPaddingAfterT<0, const Object*, std::remove_reference_t<Args>...>());
		if constexpr (std::is_void_v<ReturnType>)
			call(stack, function, objectPointer, std::index_sequence_for<Args...>{});
		else
			set(stack, call(stack, function, objectPointer, std::index_sequence_for<Args...>{}), (0 + ... + sizeof(std::remove_reference_t<Args>)));
		if constexpr (sizeof...(Args) > 0)
			stack.resize(std::size(stack) - (sizeof(std::remove_reference_t<Args>) + ...));
	}

	template<Type::Concept::TriviallyCopyable... Types>
	constexpr auto execute(const auto& src, auto... data) -> decltype(auto)
	{
		auto externalData = std::array<Type::UniqueVariant<decltype(&src), decltype(data)...>, 1 + sizeof...(data)>{ &src, data... };
		auto stack = std::vector(sizeof(std::intptr_t), std::byte{0});
		auto stackTypes = std::vector<std::size_t>{};
		const auto externalTypes = std::array<void(*)(decltype(stack)&), sizeof...(Types)>{
			+[](decltype(stack)& stack) -> void { push(stack, Types{}); }...
		};
		for (auto ip = std::size_t{0}; ip < std::size(src); ++ip)
		{
			switch (auto c = src[ip]; c)
			{
			case ',': [[fallthrough]];
			case '(': push(stack, std::intptr_t{0}); break;
			case ')': pop<std::intptr_t>(stack); break;
			case '_': set(stack, std::intptr_t{0}); break;
			case 'C':
			{
				auto value = get<std::intptr_t>(stack, (get<std::size_t>(stack, sizeof(std::intptr_t)) + 2) * sizeof(std::size_t));
				set(stack, value, (get<std::size_t>(stack) + 2) * sizeof(std::size_t));
				pop<std::intptr_t, std::intptr_t>(stack);
				break;
			}
			// case 'D': set(stack, &get<std::intptr_t>(stack)); break;
			case 'R': set(stack, *get<std::intptr_t*>(stack)); break;
			case 'J': ip = get<std::size_t>(stack); pop<std::size_t>(stack); break;
			case 'P': push(stack, ip); break;
			case ':':
			{
				auto id = get<std::size_t>(stack);
				pop<std::size_t>(stack);
				externalTypes.at(id)(stack);
				break;
			}
			case ';':
				if constexpr (sizeof...(data) > 0)
				{
					auto& data = externalData[get<std::size_t>(stack)];
					pop<std::size_t>(stack);
					std::visit([&stack](auto&& data) -> void {
						using T = std::remove_reference_t<decltype(data)>;
						if constexpr (Type::Concept::isFunctionPointer<T> || std::is_member_function_pointer_v<T>)
							call(stack, data);
						else
							push(stack, data);
					}, data);
				}
				break;
			case '?': if (!get<std::intptr_t>(stack, sizeof(std::size_t))) ip += get<std::size_t>(stack); pop<std::size_t>(stack); break;
			case '!': set(stack, std::intptr_t{!get<std::intptr_t>(stack)}); break;
			case '=': set(stack, std::intptr_t{get<std::intptr_t>(stack, sizeof(std::intptr_t)) == get<std::intptr_t>(stack)}, sizeof(std::intptr_t)); pop<std::intptr_t>(stack); break;
			case '<': set(stack, std::intptr_t{get<std::intptr_t>(stack, sizeof(std::intptr_t)) < get<std::intptr_t>(stack)}, sizeof(std::intptr_t)); pop<std::intptr_t>(stack); break;
			case '>': set(stack, std::intptr_t{get<std::intptr_t>(stack, sizeof(std::intptr_t)) > get<std::intptr_t>(stack)}, sizeof(std::intptr_t)); pop<std::intptr_t>(stack); break;
			case '&': set(stack, std::intptr_t{get<std::intptr_t>(stack, sizeof(std::intptr_t)) & get<std::intptr_t>(stack)}, sizeof(std::intptr_t)); pop<std::intptr_t>(stack); break;
			case '|': set(stack, std::intptr_t{get<std::intptr_t>(stack, sizeof(std::intptr_t)) | get<std::intptr_t>(stack)}, sizeof(std::intptr_t)); pop<std::intptr_t>(stack); break;
			case '^': set(stack, std::intptr_t{get<std::intptr_t>(stack, sizeof(std::intptr_t)) ^ get<std::intptr_t>(stack)}, sizeof(std::intptr_t)); pop<std::intptr_t>(stack); break;
			case '+': set(stack, get<std::intptr_t>(stack, sizeof(std::intptr_t)) + get<std::intptr_t>(stack), sizeof(std::intptr_t)); pop<std::intptr_t>(stack); break;
			case '-': set(stack, get<std::intptr_t>(stack, sizeof(std::intptr_t)) - get<std::intptr_t>(stack), sizeof(std::intptr_t)); pop<std::intptr_t>(stack); break;
			case '*': set(stack, get<std::intptr_t>(stack, sizeof(std::intptr_t)) * get<std::intptr_t>(stack), sizeof(std::intptr_t)); pop<std::intptr_t>(stack); break;
			case '/': set(stack, get<std::intptr_t>(stack, sizeof(std::intptr_t)) / get<std::intptr_t>(stack), sizeof(std::intptr_t)); pop<std::intptr_t>(stack); break;
			case '\\': push(stack, src[ip + 1]); break;
			case 'X': return get<std::intptr_t>(stack);
			default: if (c >= '0' && c <= '9') set(stack, get<std::intptr_t>(stack) * 10 + c - '0'); break;
			}
		}
		return get<std::intptr_t>(stack);
	}
}
