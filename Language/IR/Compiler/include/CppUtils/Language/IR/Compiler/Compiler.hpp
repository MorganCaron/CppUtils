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
			{ "comma"_token, CompilationFunctions<Address>::compileComma },
			{ "halt"_token, CompilationFunctions<Address>::compileHalt },
			{ "ident"_token, CompilationFunctions<Address>::compileIdent },
			{ "number"_token, CompilationFunctions<Address>::compileNumber },
			{ "string"_token, CompilationFunctions<Address>::compileString },
			{ "copy"_token, CompilationFunctions<Address>::compileCopy },
			{ "eq"_token, CompilationFunctions<Address>::compileEq },
			{ "add"_token, CompilationFunctions<Address>::compileAdd },
			{ "sub"_token, CompilationFunctions<Address>::compileSub },
			{ "label"_token, CompilationFunctions<Address>::compileLabel },
			{ "ret"_token, CompilationFunctions<Address>::compileRet },
			{ "deref"_token, CompilationFunctions<Address>::compileDeref },
			{ "call"_token, CompilationFunctions<Address>::compileCall },
			{ "if"_token, CompilationFunctions<Address>::compileIf },
			{ "while"_token, CompilationFunctions<Address>::compileWhile }
		}}
		{}

		inline void compile(const ASTNode& astNode, Context<Address>& context) const
		{
			m_compiler.compile(astNode, context);
		}

		[[nodiscard]] inline Context<Address> compile(const ASTNode& astNode) const
		{
			auto context = Context<Address>{std::cref(*this)};
			buildStrings(astNode, context);
			compile(astNode, context);
			return context;
		}

		[[nodiscard]] inline Context<Address> compile(std::string_view src) const
		{
			return compile(Lexer::parse<Address>(src));
		}

		void buildStrings(const ASTNode& astNode, Context<Address>& context) const
		{
			using namespace Type::Literals;
			auto strings = std::vector<std::string>{};
			astNode.forEach("string"_token, [&strings](const ASTNode& stringNode) {
				const auto& string = std::get<Type::Token>(stringNode.childs.at(0).value);
				strings.push_back(std::string{string.name} + '\0');
			});
			std::sort(strings.begin(), strings.end(), [](const auto& lhs, const auto& rhs) {
				return (lhs.size() == rhs.size()) ? (lhs < rhs) : (lhs.size() > rhs.size());
			});
			for (const auto& string : strings)
				if (context.stringConstants.find(string) == std::string::npos)
					context.stringConstants += string;
		}

	private:
		Language::Compiler::Compiler<Context<Address>, Type::Token, Address> m_compiler;
	};
}
