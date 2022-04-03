#pragma once

#include <CppUtils/Language/ASM/Compiler/Context.hpp>

namespace CppUtils::Language::ASM::Compiler
{
	class CompilationFunctions final
	{
	public:
		CompilationFunctions() = delete;

		static void compileNop([[maybe_unused]] const Lexer::ASTNode& astNode, Context& context)
		{
			context.addInstruction(context.createInstruction());
		}

		static void compileHalt([[maybe_unused]] const Lexer::ASTNode& astNode, Context& context)
		{
			using namespace Type::Literals;
			context.addInstruction(context.createInstruction("halt"_token, std::uintptr_t{0}));
			context.returnRegister = 0;
		}

		static void compileNumber(const Lexer::ASTNode& astNode, Context& context)
		{
			const auto number = std::get<std::uintptr_t>(astNode.getChildValue());
			context.returnRegister = context.newRegister();
			context.addInstruction(context.createInstruction(context.returnRegister, "", number));
		}

		static void compileString(const Lexer::ASTNode& astNode, Context& context)
		{
			const auto string = std::get<std::uintptr_t>(astNode.getChildValue());
			context.returnRegister = context.newRegister();
			context.addInstruction(context.createInstruction(context.returnRegister, "", string));
		}

		static void compileMove(const Lexer::ASTNode& astNode, Context& context)
		{
			using namespace Type::Literals;
			const auto lhs = std::get<std::uintptr_t>(astNode.getChildValue(0));
			context.compiler.get().compile(astNode.childs.at(1), context);
			const auto rhs = context.returnRegister;
			context.addInstruction(context.createInstruction("move"_token, lhs, rhs));
		}

		static void compileAdd(const Lexer::ASTNode& astNode, Context& context)
		{
			using namespace Type::Literals;
			const auto lhs = std::get<std::uintptr_t>(astNode.getChildValue(0));
			context.compiler.get().compile(astNode.childs.at(1), context);
			const auto rhs = context.returnRegister;
			context.addInstruction(context.createInstruction("add"_token, lhs, rhs));
		}

		static void compileLabel(const Lexer::ASTNode& astNode, Context& context)
		{
			context.compiler.get().compile(astNode.childs.at(1), context);
		}
	};
}
