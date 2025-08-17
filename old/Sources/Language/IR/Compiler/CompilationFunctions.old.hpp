#pragma once

#include <CppUtils/Language/IR/Compiler/Context.hpp>

namespace CppUtils::Language::IR::Compiler
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

		static void compileComma(const Lexer::ASTNode& astNode, Context& context)
		{
			for (const auto& child : astNode.children)
				context.compiler.get().compile(child, context);
		}

		static void compileIdent(const Lexer::ASTNode& astNode, Context& context)
		{
			const auto ident = std::get<Type::Token>(astNode.getChildValue());
			context.returnRegister = context.getRegister(ident);
		}

		static void compileNumber(const Lexer::ASTNode& astNode, Context& context)
		{
			const auto number = std::get<std::uintptr_t>(astNode.getChildValue());
			context.returnRegister = context.newRegister();
			context.addInstruction(context.createInstruction(context.returnRegister, "", number));
		}

		static void compileString(const Lexer::ASTNode& astNode, Context& context)
		{
			const auto string = std::get<std::string>(astNode.getChildValue());
			context.returnRegister = context.newRegister();
			context.addInstruction(context.createInstruction(context.returnRegister, "$STR", context.output.get().stringConstants.find(string + '\0')));
		}

		static void compileCopy(const Lexer::ASTNode& astNode, Context& context)
		{
			using namespace Type::Literals;
			const auto lhsIsDeref = (astNode.getChildValue(0) == "deref"_token);
			context.compiler.get().compile(lhsIsDeref ? astNode.children.at(0).children.at(0) : astNode.children.at(0), context);
			const auto lhs = context.returnRegister;
			context.compiler.get().compile(astNode.children.at(1), context);
			const auto rhs = context.returnRegister;
			context.returnRegister = lhs;
			context.addInstruction(context.createInstruction(lhsIsDeref ? "write"_token : "copy"_token, lhs, rhs));
		}

		static void compileEq(const Lexer::ASTNode& astNode, Context& context)
		{
			using namespace Type::Literals;
			context.compiler.get().compile(astNode.children.at(0), context);
			const auto lhs = context.returnRegister;
			context.compiler.get().compile(astNode.children.at(1), context);
			const auto rhs = context.returnRegister;
			context.returnRegister = lhs;
			context.addInstruction(context.createInstruction("eq"_token, lhs, rhs));
		}

		static void compileAdd(const Lexer::ASTNode& astNode, Context& context)
		{
			using namespace Type::Literals;
			context.compiler.get().compile(astNode.children.at(0), context);
			const auto lhs = context.returnRegister;
			context.compiler.get().compile(astNode.children.at(1), context);
			const auto rhs = context.returnRegister;
			context.returnRegister = lhs;
			context.addInstruction(context.createInstruction("add"_token, lhs, rhs));
		}

		static void compileSub(const Lexer::ASTNode& astNode, Context& context)
		{
			using namespace Type::Literals;
			context.compiler.get().compile(astNode.children.at(0), context);
			const auto lhs = context.returnRegister;
			context.compiler.get().compile(astNode.children.at(1), context);
			const auto rhs = context.returnRegister;
			context.returnRegister = lhs;
			context.addInstruction(context.createInstruction("sub"_token, lhs, rhs));
		}

		static void compileLabel(const Lexer::ASTNode& astNode, Context& context)
		{
			const auto& label = std::get<Type::Token>(astNode.children.at(1).value);
			context.addFunction(label, 0);
			context.compiler.get().compile(astNode.children.at(2), context);
		}

		static void compileRet(const Lexer::ASTNode& astNode, Context& context)
		{
			using namespace Type::Literals;
			context.compiler.get().compile(astNode.children.at(0), context);
			context.addInstruction(context.createInstruction("ret"_token, context.returnRegister));
		}

		static void compileDeref(const Lexer::ASTNode& astNode, Context& context)
		{
			using namespace Type::Literals;
			context.compiler.get().compile(astNode.children.at(0), context);
			context.addInstruction(context.createInstruction("read"_token, context.returnRegister));
		}

		static void compileCall(const Lexer::ASTNode& astNode, Context& context)
		{
			using namespace Type::Literals;
			const auto& functionName = std::get<Type::Token>(astNode.getChildValue(0));
			const auto returnRegister = context.newRegister(), functionLabel = context.newRegister();
			context.addInstruction(context.createInstruction(functionLabel, functionName.name));
			auto parameters = std::vector<std::uintptr_t>{returnRegister, functionLabel};
			std::transform(astNode.children.begin() + 1, astNode.children.end(), std::back_inserter(parameters), [&context](const auto& astNode) -> std::uintptr_t {
				context.compiler.get().compile(astNode, context);
				return context.returnRegister;
			});
			context.addInstruction(context.createInstruction("call"_token, std::move(parameters)));
			context.returnRegister = returnRegister;
		}

		static void compileIf(const Lexer::ASTNode& astNode, Context& context)
		{
			using namespace Type::Literals;
			context.compiler.get().compile(astNode.children.at(0), context);
			auto* ifnz = context.createInstruction("ifnz"_token, context.returnRegister);
			context.addInstruction(ifnz);
			context.compiler.get().compile(astNode.children.at(1), context);
			ifnz->conditionInstruction = ifnz->nextInstruction;
			auto* nop = context.createInstruction();
			context.addInstruction(nop);
			ifnz->nextInstruction = nop;
		}

		static void compileWhile(const Lexer::ASTNode& astNode, Context& context)
		{
			using namespace Type::Literals;
			context.compiler.get().compile(astNode.children.at(0), context);
			auto* ifnz = context.createInstruction("ifnz"_token, context.returnRegister);
			context.addInstruction(ifnz);
			context.compiler.get().compile(astNode.children.at(1), context);
			ifnz->conditionInstruction = ifnz->nextInstruction;
			auto* endThen = context.lastInstruction;
			auto* nop = context.createInstruction();
			context.addInstruction(nop);
			endThen->nextInstruction = ifnz;
			ifnz->nextInstruction = nop;
		}
	};
}
