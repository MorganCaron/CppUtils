#pragma once

#include <vector>
#include <string>

#include <CppUtils/Type/Token.hpp>
#include <CppUtils/Language/Interpreter/Cursor.hpp>

namespace CppUtils::Language::Interpreter
{
	template<typename Instruction, typename Context>
	class VirtualMachine
	{
	public:
		using Operation = std::function<void(Cursor<Instruction>&, Context&)>;

		VirtualMachine(std::unordered_map<Type::Token, Operation, Type::Token::hash_fn>&& operations = {}):
			m_operations{operations}
		{}

		inline void setInstructions(std::vector<Instruction> instructions) noexcept
		{
			m_instructions = std::move(instructions);
		}

		void run(Context& context) const
		{
			using namespace std::literals;
			auto cursor = Cursor<Instruction>{m_instructions};
			
			try
			{
				while (cursor.pos < cursor.elements.size())
				{
					const auto operation = m_operations.find(cursor.getElement().type);
					if (operation == m_operations.end())
						throw std::runtime_error{"Unknown instruction:\n" + std::string{cursor.getElement().type.name}};
					m_operations.at(cursor.getElement().type)(cursor, context);
				}
			}
			catch (const std::exception& exception)
			{
				throw std::runtime_error{"An exception occurred during code execution on the virtual machine:\n"s + exception.what()};
			}
		}

	private:
		std::unordered_map<Type::Token, Operation, Type::Token::hash_fn> m_operations;
		std::vector<Instruction> m_instructions;
	};
}
