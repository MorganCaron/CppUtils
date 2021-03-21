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
	
	template<typename Type>
	struct Instruction final
	{
		CppUtils::Type::Token type;
		std::string name;
		Type value;
		std::vector<Type> parametersId = {};
		Instruction<Type>* nextInstruction = nullptr;
		Instruction<Type>* targetInstruction = nullptr;

		explicit Instruction(Type registerId = 0, std::string_view c_name = ""sv, Type c_value = Type{}):
			type{"init"_token}, name{c_name}, value{c_value}, parametersId{registerId}
		{}

		explicit Instruction(Instruction<Type>* c_targetInstruction):
			type{"ifnz"_token}, targetInstruction{c_targetInstruction}
		{}
		
		template<typename... Parameters> requires (std::same_as<Type, Parameters> && ...)
		explicit Instruction(CppUtils::Type::Token c_type, Parameters... c_parametersId):
			type{c_type}, parametersId{std::forward<Parameters>(c_parametersId)...}
		{}
	};

	template<typename Type>
	std::ostream& operator<<(std::ostream& os, const Instruction<Type>& instruction)
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
