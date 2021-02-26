#pragma once

#include <CppUtils/Type/Token.hpp>

namespace CppUtils::Language::Compiler
{
	template<typename OutputInstruction, typename... Types>
	class Compiler final
	{
	public:
		using CompilationFunction = std::function<std::vector<OutputInstruction>(const Parser::ASTNode<Type::Token, Types...>&)>;

		explicit Compiler(std::unordered_map<Type::Token, CompilationFunction, Type::Token::hash_fn>&& compilationFunctions):
			m_compilationFunctions{compilationFunctions}
		{}

		std::vector<OutputInstruction> compile(const Parser::ASTNode<Type::Token, Types...>& ast) const
		{
			auto outputInstructions = std::vector<OutputInstruction>{};
			const auto instructions = ast.childs;
			for (const auto instruction : instructions)
			{
				const auto& instructionType = std::get<Type::Token>(instruction.value);
				if (m_compilationFunctions.find(instructionType) == m_compilationFunctions.end())
					throw std::runtime_error{"No compile function for instruction type " + std::string{instructionType.name}};
				auto newOutputInstructions = m_compilationFunctions.at(instructionType)(instruction);
				std::move(newOutputInstructions.begin(), newOutputInstructions.end(), std::back_inserter(outputInstructions));
			}
			return outputInstructions;
		}

	private:
		std::unordered_map<Type::Token, CompilationFunction, Type::Token::hash_fn> m_compilationFunctions;
	};
}
