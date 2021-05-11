#pragma once

#include <CppUtils/Type/Token.hpp>
#include <CppUtils/Language/IR/Compiler/Bytecode.hpp>
#include <CppUtils/Language/IR/VirtualMachine/Context.hpp>

namespace CppUtils::Language::IR::VirtualMachine
{
	template<typename Instruction, typename Context, typename Address>
	class Operations final
	{
	public:
		Operations() = delete;

		static void runNop([[maybe_unused]] const Instruction& instruction, Context& context)
		{
			context.goToNextInstruction();
		}

		static void runHalt([[maybe_unused]] const Instruction& instruction, Context& context)
		{
			context.halt();
		}

		static void runInit([[maybe_unused]] const Instruction& instruction, Context& context)
		{
			// context.push(instruction.name.empty() ? instruction.value : reinterpret_cast<Address>(&instruction.name));
			// context.registerFile[instruction.parametersId.at(0)] = context.registerVariables.esp - 1;
			context.goToNextInstruction();
		}

		static void runRead(const Instruction& instruction, Context& context)
		{
			const auto& register0 = instruction.parametersId.at(0);
			const auto& register1 = instruction.parametersId.at(1);
			const auto source = context.registerFile.at(register1);
			context.registerFile[register0] = context.registerFile.at(source);
			context.goToNextInstruction();
		}
		
		static void runWrite(const Instruction& instruction, Context& context)
		{
			const auto& register0 = instruction.parametersId.at(0);
			const auto& register1 = instruction.parametersId.at(1);
			const auto dest = context.registerFile.at(register0);
			context.registerFile[dest] = context.registerFile.at(register1);
			context.goToNextInstruction();
		}

		static void runCopy(const Instruction& instruction, Context& context)
		{
			const auto& register0 = instruction.parametersId.at(0);
			const auto& register1 = instruction.parametersId.at(1);
			context.registerFile[register0] = context.registerFile.at(register1);
			context.goToNextInstruction();
		}

		static void runAdd(const Instruction& instruction, Context& context)
		{
			const auto& register0 = instruction.parametersId.at(0);
			const auto& register1 = instruction.parametersId.at(1);
			context.registerFile[register0] += context.registerFile.at(register1);
			context.goToNextInstruction();
		}

		static void runSub(const Instruction& instruction, Context& context)
		{
			const auto& register0 = instruction.parametersId.at(0);
			const auto& register1 = instruction.parametersId.at(1);
			context.registerFile[register0] -= context.registerFile.at(register1);
			context.goToNextInstruction();
		}

		static void runRet([[maybe_unused]] const Instruction& instruction, Context& context)
		{
			context.jump(context.template pop<Address>());
		}

		static void runCall([[maybe_unused]] const Instruction& instruction, Context& context)
		{
			/*const auto& register0 = instruction->parametersId.at(0);
			auto label = context.registerFile.at(register0);
			const auto& functionInformations = context.compilerOutput.functions.at(Type::Token{label});
			context.stack.push(reinterpret_cast<Address>(instruction));
			instruction = functionInformations.entryPoint;*/
			context.goToNextInstruction();
		}
	};

	/*template<typename... Types>
	inline void jump(Parser::Cursor<Bytecode::Compiler::Bytecode::Instruction<Types...>>& cursor, [[maybe_unused]] Context<Types...>& context)
	{
		[[maybe_unused]] const auto& parameters = cursor.getElement().parameters;

		// cursor.position = parameters.at(0).id;
	}

	template<typename... Types>
	inline void move(Parser::Cursor<Bytecode::Compiler::Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		const auto register0 = std::get<Type::Token>(parameters.at(0));
		const auto register1 = std::get<Type::Token>(parameters.at(1));

		context.registerFile[register0] = context.registerFile.at(register1);
		++cursor.position;
	}*/

	/*
	template<typename... Types>
	inline void add(Parser::Cursor<Bytecode::Compiler::Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		const auto register0 = parameters.at(0);
		const auto register1 = parameters.at(1);
		const auto register2 = parameters.at(2);

		context.registerFile[register0] = context.registerFile.at(register1) + context.registerFile.at(register2);
		++cursor.position;
	}

	template<typename... Types>
	inline void sub(Parser::Cursor<Bytecode::Compiler::Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		const auto register0 = parameters.at(0);
		const auto register1 = parameters.at(1);

		context.registerFile[register0] -= context.registerFile.at(register1);
		++cursor.position;
	}
	*/
	
	/*template<typename... Types>
	inline void push(Parser::Cursor<Bytecode::Compiler::Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		const auto register0 = std::get<Type::Token>(parameters.at(0));

		context.stack.emplace(context.registerFile.at(register0));
		++cursor.position;
	}

	template<typename... Types>
	inline void pop(Parser::Cursor<Bytecode::Compiler::Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
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
