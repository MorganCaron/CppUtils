#pragma once

#include <CppUtils/Language/IR/Lexer/Lexer.hpp>
#include <CppUtils/Language/IR/Compiler/Bytecode.hpp>
#include <CppUtils/Language/IR/Compiler/Context.hpp>
#include <CppUtils/Language/IR/Compiler/CompilationFunctions.hpp>
#include <CppUtils/Language/Compiler/Compiler.hpp>
#include <CppUtils/Language/VirtualMachine/VirtualMachine.hpp>

namespace CppUtils::Language::IR::Compiler
{
	using namespace Type::Literals;

	class Compiler final
	{
	public:
		using ASTNode = Parser::ASTNode<Type::Token, std::uintptr_t, std::string>;

		Compiler(): m_compiler{{
			{ "nop"_token, CompilationFunctions::compileNop },
			{ "halt"_token, CompilationFunctions::compileHalt },
			{ "comma"_token, CompilationFunctions::compileComma },
			{ "ident"_token, CompilationFunctions::compileIdent },
			{ "number"_token, CompilationFunctions::compileNumber },
			{ "string"_token, CompilationFunctions::compileString },
			{ "copy"_token, CompilationFunctions::compileCopy },
			{ "eq"_token, CompilationFunctions::compileEq },
			{ "add"_token, CompilationFunctions::compileAdd },
			{ "sub"_token, CompilationFunctions::compileSub },
			{ "label"_token, CompilationFunctions::compileLabel },
			{ "ret"_token, CompilationFunctions::compileRet },
			{ "deref"_token, CompilationFunctions::compileDeref },
			{ "call"_token, CompilationFunctions::compileCall },
			{ "if"_token, CompilationFunctions::compileIf },
			{ "while"_token, CompilationFunctions::compileWhile }
		}}
		{}

		[[nodiscard]] Output compile(std::string_view src) const
		{
			auto astNode = Lexer::parse(src);
			auto output = Output{};
			buildStrings(astNode, output.stringConstants);
			auto context = Context{m_compiler, std::ref(output)};
			m_compiler.compile(astNode.childs, context);
			return output;
		}

		void buildStrings(const ASTNode& astNode, std::string& stringConstants) const
		{
			using namespace Type::Literals;
			auto strings = std::vector<std::string>{};
			astNode.forEach("string"_token, [&strings](const ASTNode& stringNode) {
				const auto& string = std::get<std::string>(stringNode.getChildValue());
				strings.push_back(string);
			});
			std::sort(strings.begin(), strings.end(), [](const auto& lhs, const auto& rhs) {
				return (lhs.size() == rhs.size()) ? (lhs < rhs) : (lhs.size() > rhs.size());
			});
			for (const auto& string : strings)
				if (stringConstants.find(string) == std::string::npos)
					stringConstants += string + '\0';
		}

	private:
		Context::Compiler m_compiler;
	};
}
