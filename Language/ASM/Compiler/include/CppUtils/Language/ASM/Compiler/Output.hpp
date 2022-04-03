#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <CppUtils/Type/Token.hpp>
#include <CppUtils/Language/ASM/Compiler/Bytecode.hpp>

namespace CppUtils::Language::ASM::Compiler
{
	struct FunctionInformations final
	{
		Bytecode::Instruction* entryPoint = nullptr;
		std::size_t nbParameters = 0;
	};
	
	struct Output final
	{
		std::vector<std::unique_ptr<Bytecode::Instruction>> instructions = {};
		std::string stringConstants = "";
		std::unordered_map<Type::Token, FunctionInformations, Type::Token::hash_fn> functions = {};
	};
}
