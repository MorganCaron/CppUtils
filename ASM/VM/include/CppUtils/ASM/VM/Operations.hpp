#pragma once

#include <CppUtils/ASM/Compiler/Bytecode.hpp>
#include <CppUtils/ASM/VM/Context.hpp>

namespace CppUtils::ASM::VM
{
	template<typename BytecodeInstruction, typename... Types>
	class Operations final
	{
	public:
		Operations() = delete;

		static void halt(Language::Parser::Cursor<BytecodeInstruction>& cursor, [[maybe_unused]] Context<Types...>& context)
		{
			cursor.position = cursor.end();
		}

		static void nop(Language::Parser::Cursor<BytecodeInstruction>& cursor, [[maybe_unused]] Context<Types...>& context)
		{
			++cursor.position;
		}

		static void move(Language::Parser::Cursor<BytecodeInstruction>& cursor, Context<Types...>& context)
		{
			auto& [registerFile, stack] = context;
			const auto& moveInstruction = cursor.getElement();
			const auto& registerAddress = std::get<Type::Token>(moveInstruction.parameters.at(0));
			const auto& rhs = moveInstruction.parameters.at(1);
			if (std::holds_alternative<Type::Token>(rhs))
				registerFile[registerAddress] = registerFile.at(std::get<Type::Token>(rhs));
			else
				registerFile[registerAddress] = std::visit([](auto&& value) -> std::variant<Types...> {
					if constexpr(std::is_same_v<std::decay_t<decltype(value)>, Type::Token>)
						throw std::runtime_error{"Incorrect format in the move statement"};
					else
						return std::variant<Types...>{std::forward<decltype(value)>(value)};
				}, rhs);
			++cursor.position;
		}

		static void add(Language::Parser::Cursor<BytecodeInstruction>& cursor, [[maybe_unused]] Context<Types...>& context)
		{
			++cursor.position;
		}
	};

	/*template<typename... Types>
	inline void jump(Language::Parser::Cursor<Bytecode::Instruction<Types...>>& cursor, [[maybe_unused]] Context<Types...>& context)
	{
		[[maybe_unused]] const auto& parameters = cursor.getElement().parameters;

		// cursor.position = parameters.at(0).id;
	}

	template<typename... Types>
	inline void move(Language::Parser::Cursor<Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		const auto register0 = std::get<Type::Token>(parameters.at(0));
		const auto register1 = std::get<Type::Token>(parameters.at(1));

		context.registerFile[register0] = context.registerFile.at(register1);
		++cursor.position;
	}*/

	/*
	template<typename... Types>
	inline void add(Language::Parser::Cursor<Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		const auto register0 = parameters.at(0);
		const auto register1 = parameters.at(1);
		const auto register2 = parameters.at(2);

		context.registerFile[register0] = context.registerFile.at(register1) + context.registerFile.at(register2);
		++cursor.position;
	}

	template<typename... Types>
	inline void sub(Language::Parser::Cursor<Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		const auto register0 = parameters.at(0);
		const auto register1 = parameters.at(1);

		context.registerFile[register0] -= context.registerFile.at(register1);
		++cursor.position;
	}
	*/
	
	/*template<typename... Types>
	inline void push(Language::Parser::Cursor<Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		const auto register0 = std::get<Type::Token>(parameters.at(0));

		context.stack.emplace(context.registerFile.at(register0));
		++cursor.position;
	}

	template<typename... Types>
	inline void pop(Language::Parser::Cursor<Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		const auto register0 = std::get<Type::Token>(parameters.at(0));

		if (context.stack.size() == 0)
			throw std::runtime_error{"Stack underflow"};

		context.registerFile[register0] = context.stack.top();
		context.stack.pop();
		++cursor.position;
	}*/
}
