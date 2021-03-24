#pragma once

#include <list>
#include <memory>
#include <unordered_map>

#include <CppUtils/Language/Compiler/Context.hpp>
#include <CppUtils/Language/IR/Compiler/Bytecode.hpp>
#include <CppUtils/Log/Logger.hpp>

namespace CppUtils::Language::IR::Compiler
{
	template<typename Address> requires std::is_integral_v<Address>
	struct FunctionInformations final
	{
		Bytecode::Instruction<Address>* entryPoint = nullptr;
		std::size_t nbParameters = 0;
	};

	template<typename Address> requires std::is_integral_v<Address>
	class Compiler;
	template<typename Address> requires std::is_integral_v<Address>
	struct Context final: public Language::Compiler::Context<Compiler<Address>, Bytecode::Instruction<Address>>
	{
		std::string stringConstants = "";
		std::unordered_map<Type::Token, FunctionInformations<Address>, Type::Token::hash_fn> functions = {};
		std::unordered_map<Type::Token, Address, Type::Token::hash_fn> variablesToRegisters = {};
		Bytecode::Instruction<Address>* lastInstruction = nullptr;
		Address registerCounter = 0;
		Address returnRegister = 0;

		explicit Context(std::reference_wrapper<const Compiler<Address>> compiler):
			Language::Compiler::Context<Compiler<Address>, Bytecode::Instruction<Address>>{compiler}
		{}

		void addFunction(const Type::Token& label, std::size_t nbParameters = 0)
		{
			if (functions.find(label) != functions.end())
				throw std::runtime_error{"Function " + std::string{label.name} + " is already defined."};
			addInstruction(Language::Compiler::Context<Compiler<Address>, Bytecode::Instruction<Address>>::createInstruction());
			functions[label] = FunctionInformations<Address>{lastInstruction, nbParameters};
		}

		void addInstruction(Bytecode::Instruction<Address>* instruction)
		{
			if (lastInstruction == nullptr)
				lastInstruction = instruction;
			else
				lastInstruction->nextInstruction = instruction;
			while (lastInstruction->nextInstruction != nullptr)
				lastInstruction = lastInstruction->nextInstruction;
		}

		[[nodiscard]] inline Address newRegister() noexcept
		{
			return registerCounter++;
		}

		[[nodiscard]] inline Address getRegister(const Type::Token& variable)
		{
			if (variablesToRegisters.find(variable) == variablesToRegisters.end())
				variablesToRegisters[variable] = newRegister();
			return variablesToRegisters.at(variable);
		}
	};

	template<typename Address>
	void logContext(const Context<Address>& context)
	{
		if (!context.stringConstants.empty())
		{
			Log::Logger::logImportant("$STR: ", false);
			Log::Logger::logInformation(std::string{context.stringConstants.begin(), context.stringConstants.end()});
		}
		
		struct InstructionStatistics
		{
			bool referred = false;
			bool done = false;
		};
		auto statistics = std::unordered_map<const Bytecode::Instruction<Address>*, InstructionStatistics>{};

		// Define labels for functions
		auto remainingInstructions = std::list<const Bytecode::Instruction<Address>*>{};
		auto labels = std::unordered_map<const Bytecode::Instruction<Address>*, std::string>{};
		for (const auto& [functionToken, functionInformations] : context.functions)
		{
			remainingInstructions.push_back(functionInformations.entryPoint);
			labels[functionInformations.entryPoint] = functionToken.name;
		}

		// Define labels for jumps
		auto newLabel = [nbLabels = std::size_t{0}]() mutable -> std::string { return 'L' + std::to_string(nbLabels++); };
		const auto addUniqueLabel = [&labels, &newLabel](const Bytecode::Instruction<Address>* instruction) -> void {
			if (labels.find(instruction) == labels.end())
				labels[instruction] = newLabel();
		};
		for (const auto& instruction : context.instructions)
		{
			if (const auto* next = instruction->nextInstruction; next != nullptr)
			{
				auto& stats = statistics[next];
				if (stats.referred)
					addUniqueLabel(next);
				stats.referred = true;
			}
			if (const auto* condition = instruction->conditionInstruction; condition != nullptr)
				addUniqueLabel(condition);
		}
		
		// Displays the instructions
		while (!remainingInstructions.empty())
		{
			const auto* instruction = remainingInstructions.front(); 
			remainingInstructions.pop_front();
			for (; instruction != nullptr; instruction = instruction->nextInstruction)
			{
				auto& stats = statistics[instruction];
				if (stats.done)
				{
					Log::Logger::logImportant("JMP " + labels.at(instruction));
					break;
				}
				stats.done = true;

				if (labels.find(instruction) != labels.end())
					Log::Logger::logImportant(labels.at(instruction) + ':');
				instruction->log(false);
				if (const auto* condition = instruction->conditionInstruction; condition != nullptr)
				{
					Log::Logger::logInformation(", ", false);
					Log::Logger::logImportant("JMP " + labels.at(condition), false);
					if (!statistics[condition].done)
						remainingInstructions.push_front(condition);
				}
				std::cout << '\n';
			}
		}
		std::cout << std::flush;
	}
}
