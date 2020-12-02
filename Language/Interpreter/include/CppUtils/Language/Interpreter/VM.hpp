#pragma once

#include <vector>
#include <string>

#include <CppUtils/Type/TypeId.hpp>
#include <CppUtils/Language/Interpreter/Cursor.hpp>

namespace CppUtils::Language::Interpreter
{
	using namespace std::literals;
	using namespace Type::Literals;

	template<typename Instruction, typename Context>
	class VM final
	{
	public:
		using Operation = std::function<void(Cursor<Instruction>&, Context&)>;

		VM(std::unordered_map<Type::TypeId, Operation, Type::TypeId::hash_fn>&& operations = {}):
			m_operations{operations}
		{}

		inline void setInstructions(std::vector<Instruction> instructions) noexcept
		{
			m_instructions = std::move(instructions);
		}

		void run(Context& context) const
		{
			auto cursor = Cursor<Instruction>{m_instructions};
			
			try
			{
				while (cursor.pos < cursor.instructions.size())
				{
					const auto operation = m_operations.find(cursor.getInstruction().type);
					if (operation == m_operations.end())
						throw std::runtime_error{"Unknown instruction:\n" + std::string{cursor.getInstruction().type.name}};
					m_operations.at(cursor.getInstruction().type)(cursor, context);
				}
			}
			catch (const std::exception& exception)
			{
				throw std::runtime_error{"Exception occurred during code execution in VM:\n"s + exception.what()};
			}
		}

	private:
		std::unordered_map<Type::TypeId, Operation, Type::TypeId::hash_fn> m_operations;
		std::vector<Instruction> m_instructions;
	};
}
