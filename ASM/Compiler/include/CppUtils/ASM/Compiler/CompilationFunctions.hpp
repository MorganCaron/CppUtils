#pragma once

#include <CppUtils/ASM/Compiler/Context.hpp>

namespace CppUtils::ASM::Compiler
{
	template<typename... Types>
	class CompilationFunctions final
	{
	public:
		CompilationFunctions() = delete;

		static Bytecode::Instruction<Types...> compileHalt([[maybe_unused]] const Language::Compiler::IRInstruction<Types...>& irInstruction)
		{
			using namespace Type::Literals;
			return Bytecode::Instruction<Types...>{
				.type = "halt"_token
			};
		}

		static Bytecode::Instruction<Types...> compileNop([[maybe_unused]] const Language::Compiler::IRInstruction<Types...>& irInstruction)
		{
			using namespace Type::Literals;
			return Bytecode::Instruction<Types...>{
				.type = "nop"_token
			};
		}

		static Bytecode::Instruction<Types...> compileMove([[maybe_unused]] const Language::Compiler::IRInstruction<Types...>& irInstruction)
		{
			using namespace Type::Literals;
			return Bytecode::Instruction<Types...>{
				.type = "move"_token,
				.parameters = std::vector<std::variant<Type::Token, Types...>>{
					irInstruction.childs.at(0).value,
					irInstruction.childs.at(1).childs.at(0).value
				}
			};
		}

		static Bytecode::Instruction<Types...> compileAdd([[maybe_unused]] const Language::Compiler::IRInstruction<Types...>& irInstruction)
		{
			using namespace Type::Literals;
			return Bytecode::Instruction<Types...>{
				.type = "add"_token,
				.parameters = std::vector<std::variant<Type::Token, Types...>>{
					irInstruction.childs.at(0).value,
					irInstruction.childs.at(1).childs.at(0).value
				}
			};
		}
	};
}
