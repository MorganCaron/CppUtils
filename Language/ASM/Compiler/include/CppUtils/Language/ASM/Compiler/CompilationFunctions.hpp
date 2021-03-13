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
			context.addInstruction(context.createInstruction("halt"_token, std::size_t{0}));
			context.returnRegister = std::size_t{0};
		}

		static void compileNumber(const Parser::ASTNode<CppUtils::Type::Token, std::size_t>& astNode, Context<Type>& context)
		{
			const auto number = std::get<std::size_t>(astNode.childs.at(0).value);
			context.returnRegister = context.newRegister();
			context.addInstruction(context.createInstruction(context.returnRegister, "", number));
		}

		static void compileString(const Parser::ASTNode<CppUtils::Type::Token, std::size_t>& astNode, Context<Type>& context)
		{
			const auto string = std::get<std::size_t>(astNode.childs.at(0).value);
			context.returnRegister = context.newRegister();
			context.addInstruction(context.createInstruction(context.returnRegister, "", string));
		}

		static void compileMove(const Parser::ASTNode<CppUtils::Type::Token, std::size_t>& astNode, Context<Type>& context)
		{
			using namespace CppUtils::Type::Literals;
			const auto lhs = std::get<std::size_t>(astNode.childs.at(0).value);
			context.compiler.get().compile(astNode.childs.at(1), context);
			const auto rhs = context.returnRegister;
			context.addInstruction(context.createInstruction("move"_token, lhs, rhs));
		}

		static void compileAdd(const Parser::ASTNode<CppUtils::Type::Token, std::size_t>& astNode, Context<Type>& context)
		{
			using namespace CppUtils::Type::Literals;
			const auto lhs = std::get<std::size_t>(astNode.childs.at(0).value);
			context.compiler.get().compile(astNode.childs.at(1), context);
			const auto rhs = context.returnRegister;
			context.addInstruction(context.createInstruction("add"_token, lhs, rhs));
		}

		static void compileLabel(const Parser::ASTNode<CppUtils::Type::Token, std::size_t>& astNode, Context<Type>& context)
		{
			context.compiler.get().compile(astNode.childs.at(1), context);
		}
	};
}
