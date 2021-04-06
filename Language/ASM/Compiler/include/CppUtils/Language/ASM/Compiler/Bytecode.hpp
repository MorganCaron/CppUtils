#pragma once

#include <string>
#include <vector>

#include <CppUtils/Type/Token.hpp>

namespace CppUtils::Language::ASM::Compiler::Bytecode
{
	using namespace std::literals;
	using namespace Type::Literals;
	
	template<typename Address>
	struct Instruction final
	{
		Type::Token type;
		std::string name;
		Address value;
		std::vector<Address> parametersId = {};
		Instruction<Address>* nextInstruction = nullptr;
		Instruction<Address>* conditionInstruction = nullptr;
		
		explicit Instruction(Type::Token c_type = "nop"_token):
			type{c_type}
		{}
		
		explicit Instruction(Address registerId, std::string_view c_name = ""sv, Address c_value = Address{}):
			type{"init"_token}, name{c_name}, value{c_value}, parametersId{registerId}
		{}
		
		template<typename... Parameters> requires (std::same_as<Address, Parameters> && ...)
		Instruction(Type::Token c_type, Parameters... c_parametersId):
			type{c_type}, parametersId{std::forward<Parameters>(c_parametersId)...}
		{}

		Instruction(Type::Token c_type, std::vector<Address> c_parametersId):
			type{c_type}, parametersId{std::move(c_parametersId)}
		{}

		void log(bool newLine = true) const
		{
			using namespace std::literals;
			Log::Logger::logInformation(std::string{type.name} + '\t', false);
			for (const auto& parameterId : parametersId)
				Log::Logger::logDetail(" R" + std::to_string(parameterId), false);
			if (type == "init"_token)
				Log::Logger::logDebug(" \""s + name + "\" " + std::to_string(value), false);
			if (newLine)
				std::cout << std::endl;
		}
	};
}
