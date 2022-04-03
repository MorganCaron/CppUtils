#pragma once

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

#include <CppUtils/Type/Token.hpp>
#include <CppUtils/Language/IR/Compiler/Bytecode.hpp>
#include <CppUtils/Log/Logger.hpp>

namespace CppUtils::Language::IR::Compiler
{
	struct FunctionInformations final
	{
		Bytecode::Instruction* entryPoint = nullptr;
		std::size_t nbParameters = 0;
	};

	struct Output final
	{
		std::vector<std::unique_ptr<Bytecode::Instruction>> instructions = {};
		std::string stringConstants = "";
		std::unordered_map<Type::Token, FunctionInformations, Type::Token::hash_fn> functions = {};

		[[nodiscard]] Bytecode::Instruction* getFunctionEntryPoint(const Type::Token& label) const
		{
			if (const auto functionIterator = functions.find(label); functionIterator != functions.end())
				return functionIterator->second.entryPoint;
			throw std::runtime_error{"Undefined label \"" + std::string{label.name} + '"'};
		}

		void log() const
		{
			if (!stringConstants.empty())
			{
				Log::Logger::logImportant("$STR: ", false);
				Log::Logger::logInformation(std::string{stringConstants.begin(), stringConstants.end()});
			}
			
			struct InstructionStatistics
			{
				bool referred = false;
				bool done = false;
			};
			auto statistics = std::unordered_map<const Bytecode::Instruction*, InstructionStatistics>{};

			// Define labels for functions
			auto remainingInstructions = std::list<const Bytecode::Instruction*>{};
			auto labels = std::unordered_map<const Bytecode::Instruction*, std::string>{};
			for (const auto& [functionToken, functionInformations] : functions)
			{
				remainingInstructions.push_back(functionInformations.entryPoint);
				labels[functionInformations.entryPoint] = functionToken.name;
			}

			// Define labels for jumps
			auto newLabel = [nbLabels = std::size_t{0}]() mutable -> std::string { return 'L' + std::to_string(nbLabels++); };
			const auto addUniqueLabel = [&labels, &newLabel](const Bytecode::Instruction* instruction) -> void {
				if (labels.find(instruction) == labels.end())
					labels[instruction] = newLabel();
			};
			for (const auto& instruction : instructions)
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
	};
}
