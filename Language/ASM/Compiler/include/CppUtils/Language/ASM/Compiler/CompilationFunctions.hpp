#pragma once

#include <CppUtils/Language/ASM/Compiler/Context.hpp>

namespace CppUtils::Language::ASM::Compiler
{
	template<typename Type>
	class CompilationFunctions final
	{
	public:
		CompilationFunctions() = delete;

		static void compileNop([[maybe_unused]] const Parser::ASTNode<CppUtils::Type::Token, std::size_t>& astNode, Context<Type>& context)
		{
			context.addInstruction(context.createInstruction());
		}

		static void compileHalt([[maybe_unused]] const Parser::ASTNode<CppUtils::Type::Token, std::size_t>& astNode, Context<Type>& context)
		{
			using namespace CppUtils::Type::Literals;
			context.addInstruction(context.createInstruction("exit"_token, std::size_t{0}));
		}

		static void compileNumber([[maybe_unused]] const Parser::ASTNode<CppUtils::Type::Token, std::size_t>& astNode, Context<Type>& context)
		{
			const auto number = std::get<std::size_t>(astNode.childs.at(0).value);
			context.addInstruction(context.createInstruction("", number));
		}

		static void compileMove(const Parser::ASTNode<CppUtils::Type::Token, std::size_t>& astNode, Context<Type>& context)
		{
			using namespace CppUtils::Type::Literals;
			Graph::logTreeNode(astNode);
			const auto lhs = std::get<std::size_t>(astNode.childs.at(0).value);
			const auto rhs = std::get<std::size_t>(astNode.childs.at(1).value);
			context.addInstruction(context.createInstruction("move"_token, lhs, rhs));
		}

		static void compileAdd(const Parser::ASTNode<CppUtils::Type::Token, std::size_t>& astNode, Context<Type>& context)
		{
			using namespace CppUtils::Type::Literals;
			const auto lhs = std::get<std::size_t>(astNode.childs.at(0).value);
			const auto rhs = std::get<std::size_t>(astNode.childs.at(1).value);
			context.addInstruction(context.createInstruction("add"_token, lhs, rhs));
		}
	};
}
