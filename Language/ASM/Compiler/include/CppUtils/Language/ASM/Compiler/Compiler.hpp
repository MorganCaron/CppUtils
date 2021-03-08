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
		Compiler(): m_compiler{{
			{ "nop"_token, CompilationFunctions<Type>::compileNop },
			{ "hlt"_token, CompilationFunctions<Type>::compileHalt },
			{ "move"_token, CompilationFunctions<Type>::compileMove },
			{ "add"_token, CompilationFunctions<Type>::compileAdd }
		}}
		{}

		[[nodiscard]] inline auto compile(const Parser::ASTNode<CppUtils::Type::Token, std::size_t>& ast, Context<Type>& context) const
		{
			return m_compiler.compile(ast, context);
		}

	private:
		Language::Compiler::Compiler<Bytecode::Instruction<Type>, Context<Type>, CppUtils::Type::Token, std::size_t> m_compiler;
	};

	template<typename Type>
	[[nodiscard]] inline auto compile(const Parser::ASTNode<CppUtils::Type::Token, std::size_t>& ast, Context<Type>& context)
	{
		static const auto compiler = Compiler<Type>{};
		return compiler.compile(ast, context);
	}

	template<typename Type>
	[[nodiscard]] inline auto compile(std::string_view src)
	{
		auto context = Context<Type>{};
		return compile<Type>(Lexer::parse<CppUtils::Type::Token, std::size_t>(src), context);
	}
}
