#pragma once

#include <span>
#include <string>

#include <CppUtils/Type/Token.hpp>

namespace CppUtils::Language::Interpreter
{
	struct Context
	{
		bool running = true;
	};

	template<typename Instruction, typename Context>
	class VirtualMachine
	{
	public:
		using Iterator = typename std::span<const Instruction>::iterator&;
		using Operation = std::function<void(Iterator, Context&)>;

		VirtualMachine(std::unordered_map<Type::Token, Operation, Type::Token::hash_fn>&& operations = {}):
			m_operations{operations}
		{}

		void run(std::span<const Instruction> instructions, Context& context) const
		{
			using namespace std::literals;
			auto instructionIterator = instructions.begin();
			
			try
			{
				while (context.running && instructionIterator != instructions.end())
				{
					const auto operation = m_operations.find(instructionIterator->type);
					if (operation == m_operations.end())
						throw std::runtime_error{"Unknown instruction:\n" + std::string{instructionIterator->type.name}};
					m_operations.at(instructionIterator->type)(instructionIterator, context);
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
