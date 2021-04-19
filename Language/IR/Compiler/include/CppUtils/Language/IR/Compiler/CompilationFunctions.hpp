#pragma once

#include <algorithm>
#include <CppUtils/Language/Parser/Context.hpp>
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

		static void compileComma(const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			for (const auto& child : astNode.childs)
				context.compiler.get().compile(child, context);
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
			const auto string = std::get<Type::Token>(astNode.childs.at(0).value);
			context.returnRegister = context.newRegister();
			context.addInstruction(context.createInstruction(context.returnRegister, "$STR", context.stringConstants.find(std::string{string.name} + '\0')));
		}

		static void compileCopy(const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			using namespace Type::Literals;
			const auto lhsIsDeref = (astNode.childs.at(0).value == "deref"_token);
			context.compiler.get().compile(lhsIsDeref ? astNode.childs.at(0).childs.at(0) : astNode.childs.at(0), context);
			const auto lhs = context.returnRegister;
			context.compiler.get().compile(astNode.childs.at(1), context);
			const auto rhs = context.returnRegister;
			context.returnRegister = lhs;
			context.addInstruction(context.createInstruction(lhsIsDeref ? "write"_token : "copy"_token, lhs, rhs));
		}

		static void compileEq(const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			using namespace Type::Literals;
			context.compiler.get().compile(astNode.childs.at(0), context);
			const auto lhs = context.returnRegister;
			context.compiler.get().compile(astNode.childs.at(1), context);
			const auto rhs = context.returnRegister;
			context.returnRegister = lhs;
			context.addInstruction(context.createInstruction("eq"_token, lhs, rhs));
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

		static void compileSub(const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			using namespace Type::Literals;
			context.compiler.get().compile(astNode.childs.at(0), context);
			const auto lhs = context.returnRegister;
			context.compiler.get().compile(astNode.childs.at(1), context);
			const auto rhs = context.returnRegister;
			context.returnRegister = lhs;
			context.addInstruction(context.createInstruction("sub"_token, lhs, rhs));
		}

		static void compileLabel(const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			const auto& label = std::get<Type::Token>(astNode.childs.at(0).value);
			context.addFunction(label, 0);
			context.compiler.get().compile(astNode.childs.at(1), context);
		}

		static void compileRet(const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			using namespace Type::Literals;
			context.compiler.get().compile(astNode.childs.at(0), context);
			context.addInstruction(context.createInstruction("ret"_token, context.returnRegister));
		}

		static void compileDeref(const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			using namespace Type::Literals;
			context.compiler.get().compile(astNode.childs.at(0), context);
			context.addInstruction(context.createInstruction("read"_token, context.returnRegister));
		}

		static void compileCall(const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			using namespace Type::Literals;
			const auto& functionName = std::get<Type::Token>(astNode.childs.at(0).value);
			context.returnRegister = context.newRegister();
			context.addInstruction(context.createInstruction(context.returnRegister, functionName.name));
			auto parameters = std::vector<Address>{context.returnRegister};
			std::transform(astNode.childs.begin() + 1, astNode.childs.end(), std::back_inserter(parameters), [&context](const auto& astNode) -> Address {
				context.compiler.get().compile(astNode, context);
				return context.returnRegister;
			});
			context.addInstruction(context.createInstruction("call"_token, std::move(parameters)));
		}

		static void compileIf(const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			using namespace Type::Literals;
			context.compiler.get().compile(astNode.childs.at(0), context);
			auto* ifnz = context.createInstruction("ifnz"_token, context.returnRegister);
			context.addInstruction(ifnz);
			context.compiler.get().compile(astNode.childs.at(1), context);
			ifnz->conditionInstruction = ifnz->nextInstruction;
			auto* nop = context.createInstruction();
			context.addInstruction(nop);
			ifnz->nextInstruction = nop;
		}

		static void compileWhile(const Parser::ASTNode<Type::Token, Address>& astNode, Context<Address>& context)
		{
			using namespace Type::Literals;
			context.compiler.get().compile(astNode.childs.at(0), context);
			auto* ifnz = context.createInstruction("ifnz"_token, context.returnRegister);
			context.addInstruction(ifnz);
			context.compiler.get().compile(astNode.childs.at(1), context);
			ifnz->conditionInstruction = ifnz->nextInstruction;
			auto* endThen = context.lastInstruction;
			auto* nop = context.createInstruction();
			context.addInstruction(nop);
			endThen->nextInstruction = ifnz;
			ifnz->nextInstruction = nop;
			
		}
	};
}
