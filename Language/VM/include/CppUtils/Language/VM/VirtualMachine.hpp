#pragma once

#include <span>
#include <string>

#include <CppUtils/Type/Token.hpp>

namespace CppUtils::Language::VM
{
	template<typename Instruction, typename Context>
	class VirtualMachine
	{
	public:
		using Operation = std::function<void(Parser::Cursor<Instruction>&, Context&)>;

		VirtualMachine(std::unordered_map<Type::Token, Operation, Type::Token::hash_fn>&& operations = {}):
			m_operations{operations}
		{}

		void run(std::span<const Instruction> instructions, Context& context) const
		{
			using namespace std::literals;
			auto cursor = Parser::Cursor<Instruction>{instructions, 0};

			try
			{
				while (!cursor.isEnd())
				{
					const auto& instruction = cursor.getElement();
					const auto operation = m_operations.find(instruction.type);
					if (operation == m_operations.end())
						throw std::runtime_error{"Unknown bytecode instruction:\n" + std::string{instruction.type.name}};
					operation->second(cursor, context);
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
