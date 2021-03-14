#pragma once

#include <CppUtils/Language/IR/Lexer/Lexer.hpp>
#include <CppUtils/Language/IR/Compiler/Bytecode.hpp>
#include <CppUtils/Language/IR/Compiler/Context.hpp>
#include <CppUtils/Language/IR/Compiler/CompilationFunctions.hpp>
#include <CppUtils/Language/Compiler/Compiler.hpp>

namespace CppUtils::Language::IR::Compiler
{
	using namespace Type::Literals;

	template<typename Address> requires std::is_integral_v<Address>
	class Compiler final
	{
	public:
		using ASTNode = Parser::ASTNode<Type::Token, Address>;

		Compiler(): m_compiler{{
			{ "nop"_token, CompilationFunctions<Address>::compileNop },
			{ "halt"_token, CompilationFunctions<Address>::compileHalt },
			{ "ident"_token, CompilationFunctions<Address>::compileIdent },
			{ "number"_token, CompilationFunctions<Address>::compileNumber },
			{ "string"_token, CompilationFunctions<Address>::compileString },
			{ "copy"_token, CompilationFunctions<Address>::compileCopy },
			{ "add"_token, CompilationFunctions<Address>::compileAdd },
			{ "neg"_token, CompilationFunctions<Address>::compileNeg },
			{ "label"_token, CompilationFunctions<Address>::compileLabel },
			{ "ret"_token, CompilationFunctions<Address>::compileRet }
		}}
		{}

		inline void compile(const ASTNode& astNode, Context<Address>& context) const
		{
			m_compiler.compile(astNode, context);
		}

		inline void compile(const std::vector<ASTNode>& astNodes, Context<Address>& context) const
		{
			m_compiler.compile(astNodes, context);
		}

		[[nodiscard]] inline Context<Address> compile(std::string_view src) const
		{
			auto context = Context<Address>{std::cref(*this)};
			compile(Lexer::parse<Address>(src).childs, context);
			return context;
		}

	private:
		Language::Compiler::Compiler<Context<Address>, Type::Token, Address> m_compiler;
	};
}
