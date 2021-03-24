#pragma once

#include <span>
#include <string>

#include <CppUtils/Type/Token.hpp>

namespace CppUtils::Language::VirtualMachine
{
	template<typename Instruction, typename Context>
	class VirtualMachine
	{
	public:
		using Operation = std::function<void(Instruction*, Context&)>;

		VirtualMachine(std::unordered_map<Type::Token, Operation, Type::Token::hash_fn>&& operations = {}):
			m_operations{operations}
		{}

		void run(std::span<const std::unique_ptr<Instruction>> instructions, Context& context) const
		{
			using namespace std::literals;
			try
			{
				auto instruction = instructions[0].get();
				while (instruction != nullptr)
				{
					const auto operation = m_operations.find(instruction->type);
					if (operation == m_operations.end())
						throw std::runtime_error{"Unknown bytecode instruction:\n" + std::string{instruction->type.name}};
					operation->second(instruction, context);
				}
			}
			catch (const std::exception& exception)
			{
				throw std::runtime_error{"An exception occurred during code execution on the virtual machine:\n"s + exception.what()};
			}
		}

	private:
		std::unordered_map<Type::Token, Operation, Type::Token::hash_fn> m_operations;
	};
}
