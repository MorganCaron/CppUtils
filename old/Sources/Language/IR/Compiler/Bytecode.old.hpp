#pragma once

#include <concepts>
#include <string>
#include <vector>

#include <CppUtils/Log/Logger.hpp>
#include <CppUtils/String/Hash.hpp>

namespace CppUtils::Language::IR::Compiler::Bytecode
{
	using namespace std::literals;
	using namespace String::Literals;

	struct Instruction final
	{
		String::Token type;
		std::string name;
		std::uintptr_t value = 0;
		std::vector<std::uintptr_t> parametersId = {};
		Instruction* nextInstruction = nullptr;
		Instruction* conditionInstruction = nullptr;

		explicit Instruction(String::Token c_type = "nop"_token):
			type{c_type}
		{}

		explicit Instruction(std::uintptr_t registerId, std::string_view c_name = ""sv, std::uintptr_t c_value = 0):
			type{"init"_token}, name{c_name}, value{c_value}, parametersId{registerId}
		{}

		template<class... Parameters>
		requires (std::same_as<std::uintptr_t, Parameters> and ...)
		Instruction(String::Token c_type, Parameters... c_parametersId):
			type{c_type}, parametersId{std::forward<Parameters>(c_parametersId)...}
		{}

		Instruction(String::Token c_type, std::vector<std::uintptr_t> c_parametersId):
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
				std::cout << '\n';
			std::cout << std::flush;
		}
	};
}
