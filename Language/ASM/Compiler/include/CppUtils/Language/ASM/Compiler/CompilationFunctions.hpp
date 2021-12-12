#pragma once

#include <CppUtils/Language/ASM/Compiler/Context.hpp>

namespace CppUtils::Language::ASM::Compiler
{
	template<typename Address>
	requires Type::Traits::isAddress<Address>
	class CompilationFunctions final
	{
	public:
		CompilationFunctions() = delete;

		static void compileNop([[maybe_unused]] const Lexer::ASTNode<Address>& astNode, Context<Address>& context)
		{
			context.addInstruction(context.createInstruction());
		}

		static void compileHalt([[maybe_unused]] const Lexer::ASTNode<Address>& astNode, Context<Address>& context)
		{
			using namespace Type::Literals;
			context.addInstruction(context.createInstruction("halt"_token, Address{0}));
			context.returnRegister = Address{0};
		}

		static void compileNumber(const Lexer::ASTNode<Address>& astNode, Context<Address>& context)
		{
			const auto number = std::get<Address>(astNode.getChildValue());
			context.returnRegister = context.newRegister();
			context.addInstruction(context.createInstruction(context.returnRegister, "", number));
		}

		static void compileString(const Lexer::ASTNode<Address>& astNode, Context<Address>& context)
		{
			const auto string = std::get<Address>(astNode.getChildValue());
			context.returnRegister = context.newRegister();
			context.addInstruction(context.createInstruction(context.returnRegister, "", string));
		}

		static void compileMove(const Lexer::ASTNode<Address>& astNode, Context<Address>& context)
		{
			using namespace Type::Literals;
			const auto lhs = std::get<Address>(astNode.getChildValue(0));
			context.compiler.get().compile(astNode.childs.at(1), context);
			const auto rhs = context.returnRegister;
			context.addInstruction(context.createInstruction("move"_token, lhs, rhs));
		}

		static void compileAdd(const Lexer::ASTNode<Address>& astNode, Context<Address>& context)
		{
			using namespace Type::Literals;
			const auto lhs = std::get<Address>(astNode.getChildValue(0));
			context.compiler.get().compile(astNode.childs.at(1), context);
			const auto rhs = context.returnRegister;
			context.addInstruction(context.createInstruction("add"_token, lhs, rhs));
		}

		static void compileLabel(const Lexer::ASTNode<Address>& astNode, Context<Address>& context)
		{
			context.compiler.get().compile(astNode.childs.at(1), context);
		}
	};
}
