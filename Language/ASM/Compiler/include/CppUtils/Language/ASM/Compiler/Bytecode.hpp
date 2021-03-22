#pragma once

#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <iostream>

#include <CppUtils/Type/Token.hpp>

namespace CppUtils::Language::ASM::Compiler::Bytecode
{
	using namespace std::literals;
	using namespace Type::Literals;
	
	template<typename Address>
	struct Instruction final
	{
		CppUtils::Type::Token type;
		std::string name;
		Address value;
		std::vector<Address> parametersId = {};
		Instruction<Address>* nextInstruction = nullptr;
		Instruction<Address>* conditionInstruction = nullptr;
		
		explicit Instruction(Address registerId = 0, std::string_view c_name = ""sv, Address c_value = Address{}):
			type{"init"_token}, name{c_name}, value{c_value}, parametersId{registerId}
		{}
		
		template<typename... Parameters> requires (std::same_as<Address, Parameters> && ...)
		explicit Instruction(CppUtils::Type::Token c_type = "nop"_token, Parameters... c_parametersId):
			type{c_type}, parametersId{std::forward<Parameters>(c_parametersId)...}
		{}
	};

	template<typename Address>
	std::ostream& operator<<(std::ostream& os, const Instruction<Address>& instruction)
	{
		using namespace CppUtils::Type::Literals;
		os << instruction.type;
		for (const auto& parameterId : instruction.parametersId)
			os << " R" << parameterId;
		if (instruction.type == "init"_token)
			os << " \"" << instruction.name << "\" " << instruction.value;
		return os;
	}
}
