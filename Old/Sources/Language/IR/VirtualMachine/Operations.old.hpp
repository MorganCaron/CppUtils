#pragma once

#include <CppUtils/Language/IR/Compiler/Bytecode.hpp>
#include <CppUtils/Language/IR/VirtualMachine/Context.hpp>
#include <CppUtils/String/Hash.hpp>

namespace CppUtils::Language::IR::VirtualMachine
{
	template<class Context>
	class Operations final
	{
	public:
		using Instruction = Compiler::Bytecode::Instruction;

		Operations() = delete;

		static void runNop([[maybe_unused]] const Instruction& instruction, Context& context)
		{
			context.programMemory.goToNextInstruction();
		}

		static void runHalt([[maybe_unused]] const Instruction& instruction, Context& context)
		{
			context.programMemory.halt();
		}

		static void runInit([[maybe_unused]] const Instruction& instruction, Context& context)
		{
			auto& [compilerOutput, programMemory] = context;
			const auto isNumber = instruction.name.empty();
			if (isNumber)
				programMemory.push(instruction.value);
			else if (instruction.name == "$STR")
				programMemory.push(reinterpret_cast<std::uintptr_t>(compilerOutput.stringConstants.data() + instruction.value));
			else
				programMemory.push(reinterpret_cast<std::uintptr_t>(&instruction.name));
			programMemory.registerFile[instruction.parametersId.at(0)] = isNumber ? instruction.value : programMemory.registerVariables.stackPointer;
			programMemory.goToNextInstruction();
		}

		static void runRead(const Instruction& instruction, Context& context)
		{
			auto& [compilerOutput, programMemory] = context;
			const auto& register0 = instruction.parametersId.at(0);
			const auto& register1 = instruction.parametersId.at(1);
			const auto source = programMemory.registerFile.at(register1);
			programMemory.registerFile[register0] = programMemory.registerFile.at(source);
			programMemory.goToNextInstruction();
		}

		static void runWrite(const Instruction& instruction, Context& context)
		{
			auto& [compilerOutput, programMemory] = context;
			const auto& register0 = instruction.parametersId.at(0);
			const auto& register1 = instruction.parametersId.at(1);
			const auto dest = programMemory.registerFile.at(register0);
			programMemory.registerFile[dest] = programMemory.registerFile.at(register1);
			programMemory.goToNextInstruction();
		}

		static void runCopy(const Instruction& instruction, Context& context)
		{
			auto& [compilerOutput, programMemory] = context;
			const auto& register0 = instruction.parametersId.at(0);
			const auto& register1 = instruction.parametersId.at(1);
			programMemory.registerFile[register0] = programMemory.registerFile.at(register1);
			programMemory.goToNextInstruction();
		}

		static void runAdd(const Instruction& instruction, Context& context)
		{
			auto& [compilerOutput, programMemory] = context;
			const auto& register0 = instruction.parametersId.at(0);
			const auto& register1 = instruction.parametersId.at(1);
			programMemory.registerFile[register0] += programMemory.registerFile.at(register1);
			programMemory.goToNextInstruction();
		}

		static void runSub(const Instruction& instruction, Context& context)
		{
			auto& [compilerOutput, programMemory] = context;
			const auto& register0 = instruction.parametersId.at(0);
			const auto& register1 = instruction.parametersId.at(1);
			programMemory.registerFile[register0] -= programMemory.registerFile.at(register1);
			programMemory.goToNextInstruction();
		}

		static void runCall(const Instruction& instruction, Context& context)
		{
			auto& [compilerOutput, programMemory] = context;
			const auto& register1 = instruction.parametersId.at(1);
			auto stackAddress = programMemory.registerFile.at(register1);
			auto labelAddress = programMemory.template getStackAddress<std::uintptr_t>(stackAddress);
			auto label = Type::Token{*reinterpret_cast<std::string*>(labelAddress)};
			programMemory.call(reinterpret_cast<std::uintptr_t>(compilerOutput.getFunctionEntryPoint(label)));
			programMemory.enter((instruction.parametersId.size() - 2) * programMemory.template countNecessaryMemoryCells<std::uintptr_t>());
		}

		static void runRet(const Instruction& instruction, Context& context)
		{
			auto& [compilerOutput, programMemory] = context;
			const auto& register0 = instruction.parametersId.at(0);
			programMemory.leave();
			programMemory.ret(programMemory.registerFile.at(register0));
		}
	};

	/*template<class... Types>
	void jump(Parser::Cursor<Bytecode::Compiler::Bytecode::Instruction<Types...>>& cursor, [[maybe_unused]] Context<Types...>& context)
	{
		[[maybe_unused]] const auto& parameters = cursor.getElement().parameters;

		// cursor.position = parameters.at(0).id;
	}

	template<class... Types>
	void move(Parser::Cursor<Bytecode::Compiler::Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		const auto register0 = std::get<Type::Token>(parameters.at(0));
		const auto register1 = std::get<Type::Token>(parameters.at(1));

		context.registerFile[register0] = context.registerFile.at(register1);
		++cursor.position;
	}*/

	/*
	template<class... Types>
	void add(Parser::Cursor<Bytecode::Compiler::Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		const auto register0 = parameters.at(0);
		const auto register1 = parameters.at(1);
		const auto register2 = parameters.at(2);

		context.registerFile[register0] = context.registerFile.at(register1) + context.registerFile.at(register2);
		++cursor.position;
	}

	template<class... Types>
	void sub(Parser::Cursor<Bytecode::Compiler::Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		const auto register0 = parameters.at(0);
		const auto register1 = parameters.at(1);

		context.registerFile[register0] -= context.registerFile.at(register1);
		++cursor.position;
	}
	*/

	/*template<class... Types>
	void push(Parser::Cursor<Bytecode::Compiler::Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
	{
		const auto& parameters = cursor.getElement().parameters;
		const auto register0 = std::get<Type::Token>(parameters.at(0));

		context.stack.emplace(context.registerFile.at(register0));
		++cursor.position;
	}

	template<class... Types>
	void pop(Parser::Cursor<Bytecode::Compiler::Bytecode::Instruction<Types...>>& cursor, Context<Types...>& context)
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
