#pragma once

#include <CppUtils/Language/ASM/Lexer/Lexer.hpp>
#include <CppUtils/Language/ASM/Compiler/Bytecode.hpp>
#include <CppUtils/Language/ASM/Compiler/Context.hpp>
#include <CppUtils/Language/ASM/Compiler/CompilationFunctions.hpp>
#include <CppUtils/Language/Compiler/Compiler.hpp>
#include <CppUtils/Language/VirtualMachine/VirtualMachine.hpp>

namespace CppUtils::Language::ASM::Compiler
{
	using namespace Type::Literals;

	template<typename Address>
	requires Type::Traits::isAddress<Address>
	class Compiler final
	{
	public:
		Compiler(): m_compiler{{
			{ "nop"_token, CompilationFunctions<Address>::compileNop },
			{ "halt"_token, CompilationFunctions<Address>::compileHalt },
			{ "number"_token, CompilationFunctions<Address>::compileNumber },
			{ "string"_token, CompilationFunctions<Address>::compileString },
			{ "move"_token, CompilationFunctions<Address>::compileMove },
			{ "add"_token, CompilationFunctions<Address>::compileAdd },
			{ "label"_token, CompilationFunctions<Address>::compileLabel }
		}}
		{}

		inline void compile(const Lexer::ASTNode<Address>& astNode, Context<Address>& context) const
		{
			m_compiler.compile(astNode, context);
		}

		[[nodiscard]] inline Output<Address> compile(std::string_view src) const
		{
			auto astNode = Lexer::parse<Address>(src);
			auto output = Output<Address>{};
			auto context = Context<Address>{std::cref(*this), std::ref(output)};
			m_compiler.compile(astNode.childs, context);
			return std::move(output);
		}

	private:
		Language::Compiler::Compiler<Lexer::ASTNode<Address>, Context<Address>> m_compiler;
	};
}
