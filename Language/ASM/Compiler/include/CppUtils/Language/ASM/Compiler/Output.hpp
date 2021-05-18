#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <CppUtils/Type/Token.hpp>
#include <CppUtils/Type/Traits.hpp>
#include <CppUtils/Language/ASM/Compiler/Bytecode.hpp>

namespace CppUtils::Language::ASM::Compiler
{
	template<typename Address>
	requires Type::Traits::isAddress<Address>
	struct FunctionInformations final
	{
		Bytecode::Instruction<Address>* entryPoint = nullptr;
		std::size_t nbParameters = 0;
	};

	template<typename Address>
	requires Type::Traits::isAddress<Address>
	struct Output final
	{
		std::vector<std::unique_ptr<Bytecode::Instruction<Address>>> instructions = {};
		std::string stringConstants = "";
		std::unordered_map<Type::Token, FunctionInformations<Address>, Type::Token::hash_fn> functions = {};
	};
}
