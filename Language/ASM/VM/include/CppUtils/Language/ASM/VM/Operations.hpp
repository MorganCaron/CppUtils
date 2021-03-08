#pragma once

#include <CppUtils/Language/ASM/Compiler/Bytecode.hpp>
#include <CppUtils/Language/ASM/VM/Context.hpp>

namespace CppUtils::Language::ASM::VM
{
	template<typename Instruction, typename Type>
	class Operations final
	{
	public:
		Operations() = delete;

		static void move(Instruction* instruction, Context<Type>& context)
		{
			auto& [registerFile, stack] = context;
			const auto& register0 = instruction->parametersId.at(0);
			const auto& register1 = instruction->parametersId.at(1);
			registerFile[register0] = registerFile.at(register1);
			instruction = instruction->nextInstruction;
		}

		static void add(Instruction* instruction, Context<Type>& context)
		{
			auto& [registerFile, stack] = context;
			const auto& register0 = instruction->parametersId.at(0);
			const auto& register1 = instruction->parametersId.at(1);
			registerFile[register0] = registerFile.at(register0) + registerFile.at(register1);
			instruction = instruction->nextInstruction;
		}
	};

	/*template<typename... Types>
	inline void jump(Parser::Cursor<Bytecode::Instruction<Types...>>& cursor, [[maybe_unused]] Context<Types...>& context)
	{
		[[maybe_unused]] const auto& parameters = cursor.getElement().parameters;

		// cursor.position = parameters.at(0).id;
	}

	template<typename... Types>
	inline void move(Parser::Cursor<Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		const auto register0 = std::get<Type::Token>(parameters.at(0));
		const auto register1 = std::get<Type::Token>(parameters.at(1));

		context.registerFile[register0] = context.registerFile.at(register1);
		++cursor.position;
	}*/

	/*
	template<typename... Types>
	inline void add(Parser::Cursor<Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		const auto register0 = parameters.at(0);
		const auto register1 = parameters.at(1);
		const auto register2 = parameters.at(2);

		context.registerFile[register0] = context.registerFile.at(register1) + context.registerFile.at(register2);
		++cursor.position;
	}

	template<typename... Types>
	inline void sub(Parser::Cursor<Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		const auto register0 = parameters.at(0);
		const auto register1 = parameters.at(1);

		context.registerFile[register0] -= context.registerFile.at(register1);
		++cursor.position;
	}
	*/
	
	/*template<typename... Types>
	inline void push(Parser::Cursor<Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		const auto register0 = std::get<Type::Token>(parameters.at(0));

		context.stack.emplace(context.registerFile.at(register0));
		++cursor.position;
	}

	template<typename... Types>
	inline void pop(Parser::Cursor<Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
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
