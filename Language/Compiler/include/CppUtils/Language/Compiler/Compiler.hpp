#pragma once

#include <CppUtils/Type/Token.hpp>

namespace CppUtils::Language::Compiler
{
	template<typename... Types>
	using IRInstruction = Graph::VariantTreeNode<Type::Token, Types...>;

	template<typename BytecodeInstruction, typename... Types>
	class Compiler final
	{
	public:
		using CompilationFunction = std::function<BytecodeInstruction(const IRInstruction<Types...>&)>;

		explicit Compiler(std::unordered_map<Type::Token, CompilationFunction, Type::Token::hash_fn>&& compilationFunctions):
			m_compilationFunctions{compilationFunctions}
		{}

		std::vector<BytecodeInstruction> compile(const IRInstruction<Types...>& ast) const
		{
			auto bytecode = std::vector<BytecodeInstruction>{};
			const auto instructions = ast.childs;
			for (const auto instruction : instructions)
			{
				const auto& instructionType = std::get<Type::Token>(instruction.value);
				if (m_compilationFunctions.find(instructionType) == m_compilationFunctions.end())
					throw std::runtime_error{"No compile function for instruction type " + std::string{instructionType.name}};
				bytecode.emplace_back(m_compilationFunctions.at(instructionType)(instruction));
			}
			return bytecode;
		}

	private:
		std::unordered_map<Type::Token, CompilationFunction, Type::Token::hash_fn> m_compilationFunctions;
	};
}
