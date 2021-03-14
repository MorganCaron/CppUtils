#pragma once

#include <CppUtils/Language/IR/Compiler/Context.hpp>

namespace CppUtils::Language::IR::Compiler
{
	template<typename Address> requires std::is_integral_v<Address>
	class CompilationFunctions final
	{
	public:
		CompilationFunctions() = delete;

		static void compileNop([[maybe_unused]] const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			context.addInstruction(context.createInstruction());
		}

		static void compileHalt([[maybe_unused]] const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			using namespace Type::Literals;
			context.addInstruction(context.createInstruction("halt"_token, Address{0}));
			context.returnRegister = Address{0};
		}

		static void compileIdent(const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			const auto ident = std::get<Type::Token>(astNode.childs.at(0).value);
			context.returnRegister = context.getRegister(ident);
		}

		static void compileNumber(const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			const auto number = std::get<Address>(astNode.childs.at(0).value);
			context.returnRegister = context.newRegister();
			context.addInstruction(context.createInstruction(context.returnRegister, "", number));
		}

		static void compileString(const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			const auto string = std::get<Address>(astNode.childs.at(0).value);
			context.returnRegister = context.newRegister();
			context.addInstruction(context.createInstruction(context.returnRegister, "", string));
		}

		static void compileCopy(const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			using namespace Type::Literals;
			context.compiler.get().compile(astNode.childs.at(0), context);
			const auto lhs = context.returnRegister;
			context.compiler.get().compile(astNode.childs.at(1), context);
			const auto rhs = context.returnRegister;
			context.returnRegister = lhs;
			context.addInstruction(context.createInstruction("copy"_token, lhs, rhs));
		}

		static void compileAdd(const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			using namespace Type::Literals;
			context.compiler.get().compile(astNode.childs.at(0), context);
			const auto lhs = context.returnRegister;
			context.compiler.get().compile(astNode.childs.at(1), context);
			const auto rhs = context.returnRegister;
			context.returnRegister = lhs;
			context.addInstruction(context.createInstruction("add"_token, lhs, rhs));
		}

		static void compileNeg(const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			using namespace Type::Literals;
			context.compiler.get().compile(astNode.childs.at(0), context);
			const auto lhs = context.returnRegister;
			context.compiler.get().compile(astNode.childs.at(1), context);
			const auto rhs = context.returnRegister;
			context.returnRegister = lhs;
			context.addInstruction(context.createInstruction("neg"_token, lhs, rhs));
		}

		static void compileLabel(const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			const auto& label = std::get<Type::Token>(astNode.childs.at(0).value);
			context.addFunction(label, 0);
			context.compiler.get().compile(astNode.childs.at(0).childs, context);
		}

		static void compileRet(const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			using namespace Type::Literals;
			context.compiler.get().compile(astNode.childs.at(0), context);
			context.addInstruction(context.createInstruction("ret"_token, context.returnRegister));
		}
	};
}
