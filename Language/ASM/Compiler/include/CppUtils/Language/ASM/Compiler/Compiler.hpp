#pragma once

#include <CppUtils/Language/ASM/Lexer/Lexer.hpp>
#include <CppUtils/Language/ASM/Compiler/Bytecode.hpp>
#include <CppUtils/Language/ASM/Compiler/Context.hpp>
#include <CppUtils/Language/ASM/Compiler/CompilationFunctions.hpp>

namespace CppUtils::Language::ASM::Compiler
{
	using namespace Type::Literals;

	template<typename Type>
	class Compiler final
	{
	public:
		using ASTNode = Parser::ASTNode<CppUtils::Type::Token, std::size_t>;

		Compiler(): m_compiler{{
			{ "nop"_token, CompilationFunctions<Type>::compileNop },
			{ "halt"_token, CompilationFunctions<Type>::compileHalt },
			{ "number"_token, CompilationFunctions<Type>::compileNumber },
			{ "move"_token, CompilationFunctions<Type>::compileMove },
			{ "add"_token, CompilationFunctions<Type>::compileAdd }
		}}
		{}

		inline void compile(const ASTNode& astNode, Context<Type>& context) const
		{
			m_compiler.compile(astNode, context);
		}

		inline void compile(const std::vector<ASTNode>& astNodes, Context<Type>& context) const
		{
			m_compiler.compile(astNodes, context);
		}

		[[nodiscard]] inline Context<Type> compile(std::string_view src) const
		{
			auto context = Context<Type>{};
			compile(Lexer::parse<CppUtils::Type::Token, std::size_t>(src).childs, context);
			return context;
		}

	private:
		Language::Compiler::Compiler<Bytecode::Instruction<Type>, Context<Type>, CppUtils::Type::Token, std::size_t> m_compiler;
	};
}
