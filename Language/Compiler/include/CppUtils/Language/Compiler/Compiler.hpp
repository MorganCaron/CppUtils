#pragma once

#include <CppUtils/Type/Token.hpp>
#include <CppUtils/Type/Traits.hpp>
#include <CppUtils/Language/Parser/Context.hpp>
#include <CppUtils/Language/Compiler/Context.hpp>

namespace CppUtils::Language::Compiler
{
	template<typename Context, typename... Types>
	requires Type::Traits::isPresent<Type::Token, Types...>
	class Compiler final
	{
	public:
		using CompilationFunction = std::function<void(const Parser::ASTNode<Types...>&, Context&)>;

		explicit Compiler(std::unordered_map<Type::Token, CompilationFunction, Type::Token::hash_fn> compilationFunctions):
			m_compilationFunctions{std::move(compilationFunctions)}
		{}

		void compile(const Parser::ASTNode<Types...>& astNode, Context& context) const
		{
			if (!std::holds_alternative<Type::Token>(astNode.value))
				throw std::runtime_error{"The AST node to compile must be a token."};
			const auto& nodeType = std::get<Type::Token>(astNode.value);
			if (m_compilationFunctions.find(nodeType) == m_compilationFunctions.end())
				throw std::runtime_error{"No compile function for AST node \"" + std::string{nodeType.name} + '"'};
			try
			{
				m_compilationFunctions.at(nodeType)(astNode, context);
			}
			catch (const std::exception& exception)
			{
				throw std::runtime_error{"In the " + std::string{nodeType.name} + " compilation function:\n" + exception.what()};
			}
		}

	private:
		std::unordered_map<Type::Token, CompilationFunction, Type::Token::hash_fn> m_compilationFunctions;
	};
}
