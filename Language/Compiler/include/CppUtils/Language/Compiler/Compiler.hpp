#pragma once

#include <CppUtils/Type/Token.hpp>
#include <CppUtils/Language/Compiler/Context.hpp>

namespace CppUtils::Language::Compiler
{
	template<typename Instruction, typename Context, typename... Types>
	requires (std::is_base_of<Compiler::Context<Instruction>, Context>::value && Type::Concept::isPresent<Type::Token, Types...>)
	class Compiler final
	{
	public:
		using CompilationFunction = std::function<void(const Parser::ASTNode<Types...>&, Context&)>;

		explicit Compiler(std::unordered_map<Type::Token, CompilationFunction, Type::Token::hash_fn> compilationFunctions):
			m_compilationFunctions{std::move(compilationFunctions)}
		{}

		void compile(const Parser::ASTNode<Types...>& astNode, Context& context) const
		{
			const auto& nodeType = std::get<Type::Token>(astNode.value);
			if (m_compilationFunctions.find(nodeType) == m_compilationFunctions.end())
				throw std::runtime_error{"No compile function for AST node \"" + std::string{nodeType.name} + '"'};
			m_compilationFunctions.at(nodeType)(astNode, context);
		}

		[[nodiscard]] std::vector<std::unique_ptr<Instruction>> compile(const std::vector<Parser::ASTNode<Types...>>& astNodes, Context& context) const
		{
			for (const auto astNode : astNodes)
				compile(astNode, context);
			return std::move(context.instructions);
		}

	private:
		std::unordered_map<Type::Token, CompilationFunction, Type::Token::hash_fn> m_compilationFunctions;
	};
}
