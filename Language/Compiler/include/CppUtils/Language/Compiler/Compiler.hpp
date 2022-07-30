#pragma once

#include <CppUtils/Hash/Token.hpp>
#include <CppUtils/Language/Parser/Context.hpp>
#include <CppUtils/Language/VirtualMachine/VirtualMachine.hpp>

namespace CppUtils::Language::Compiler
{
	template<class ASTNode, class Context>
	class Compiler final
	{
	public:
		using CompilationFunction = typename VirtualMachine::VirtualMachine<ASTNode, Context>::Operation;

		explicit Compiler(std::unordered_map<Type::Token, CompilationFunction, Type::Token::hash_fn> compilationFunctions):
			m_virtualMachine{std::move(compilationFunctions)}
		{}

		void compile(std::span<const ASTNode> astNodes, Context& context) const
		{
			for (const auto& astNode : astNodes)
				compile(astNode, context);
		}

		void compile(const ASTNode& astNode, Context& context) const
		{
			if (!std::holds_alternative<Type::Token>(astNode.value))
				throw std::runtime_error{"The AST node representing a compiler function must be a token."};
			const auto& nodeType = std::get<Type::Token>(astNode.value);
			try
			{
				m_virtualMachine.run(nodeType, astNode, context);
			}
			catch (const std::exception& exception)
			{
				std::throw_with_nested(std::runtime_error{"In the compiler:\nIn the " + std::string{nodeType.name} + " compilation function:"});
			}
		}

	private:
		VirtualMachine::VirtualMachine<ASTNode, Context> m_virtualMachine;
	};
}
