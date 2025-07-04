#pragma once

#include <CppUtils/Language/ASM/Compiler/Bytecode.hpp>
#include <CppUtils/Language/ASM/Compiler/CompilationFunctions.hpp>
#include <CppUtils/Language/ASM/Compiler/Context.hpp>
#include <CppUtils/Language/ASM/Lexer/Lexer.hpp>
#include <CppUtils/Language/Compiler/Compiler.hpp>
#include <CppUtils/Language/VirtualMachine/VirtualMachine.hpp>

namespace CppUtils::Language::ASM::Compiler
{
	using namespace Type::Literals;

	class Compiler final
	{
	public:
		Compiler():
			m_compiler{{{"nop"_token, CompilationFunctions::compileNop},
				{"halt"_token, CompilationFunctions::compileHalt},
				{"number"_token, CompilationFunctions::compileNumber},
				{"string"_token, CompilationFunctions::compileString},
				{"move"_token, CompilationFunctions::compileMove},
				{"add"_token, CompilationFunctions::compileAdd},
				{"label"_token, CompilationFunctions::compileLabel}}}
		{}

		void compile(const Lexer::ASTNode& astNode, Context& context) const
		{
			m_compiler.compile(astNode, context);
		}

		[[nodiscard]] Output compile(std::string_view src) const
		{
			auto astNode = Lexer::parse(src);
			auto output = Output{};
			auto context = Context{m_compiler, std::ref(output)};
			m_compiler.compile(astNode.childs, context);
			return output;
		}

	private:
		Context::Compiler m_compiler;
	};
}
