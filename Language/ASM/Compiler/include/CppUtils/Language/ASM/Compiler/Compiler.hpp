#pragma once

#include <CppUtils/Language/ASM/Lexer/Lexer.hpp>
#include <CppUtils/Language/ASM/Compiler/Bytecode.hpp>
#include <CppUtils/Language/ASM/Compiler/CompilationFunctions.hpp>

namespace CppUtils::Language::ASM::Compiler
{
	using namespace Type::Literals;

	template<typename... Types>
	class Compiler final
	{
	public:
		Compiler(): m_compiler{{
			{ "halt"_token, CompilationFunctions<Types...>::compileHalt },
			{ "nop"_token, CompilationFunctions<Types...>::compileNop },
			{ "move"_token, CompilationFunctions<Types...>::compileMove },
			{ "add"_token, CompilationFunctions<Types...>::compileAdd }
		}}
		{}

		[[nodiscard]] inline auto compile(std::string_view src) const
		{
			return compile(parse<Types...>(src));
		}

		[[nodiscard]] inline auto compile(const Parser::ASTNode<Type::Token, Types...>& ast) const
		{
			return m_compiler.compile(ast);
		}

	private:
		Language::Compiler::Compiler<Bytecode::Instruction<Types...>, Types...> m_compiler;
	};
}
