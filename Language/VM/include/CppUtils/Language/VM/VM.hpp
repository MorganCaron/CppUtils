#pragma once

#include <stack>
#include <vector>
#include <string>
#include <type_traits>
#include <functional>

#include <CppUtils/Language/VM/Bytecode.hpp>
#include <CppUtils/Language/VM/Cursor.hpp>

namespace CppUtils::Language::VM
{
	using namespace std::literals;
	using Operation = std::function<void(Cursor&, std::stack<Bytecode::Instruction>&)>;

	class VM final
	{
	public:
		VM(std::vector<Bytecode::Instruction> instructions = {}):
			m_instructions{std::move(instructions)},
			m_operations{
				{ Bytecode::InstructionType::NOP, Operations::nop }
			}
		{}

		inline void setInstructions(std::vector<Bytecode::Instruction> instructions) noexcept
		{
			m_instructions = std::move(instructions);
		}

		void run() const
		{
			if (m_instructions.empty())
				throw std::runtime_error{"No instructions have been passed to the VM."};
			auto stack = std::stack<Bytecode::Instruction>{};
			auto cursor = Cursor{m_instructions};
			auto instruction = Bytecode::Instruction{};
			try
			{
				while ((instruction = m_instructions.at(cursor.pos)).type != Bytecode::InstructionType::HLT)
					m_operations.at(instruction.type)(cursor, stack);
			}
			catch (const std::exception& exception)
			{
				throw std::runtime_error{"Exception occurred during code execution in VM:\n"s + exception.what()};
			}
		}

	private:
		struct hash_fn final
		{
			[[nodiscard]] inline constexpr std::size_t operator()(const Bytecode::InstructionType& nb) const noexcept
			{
				return static_cast<std::underlying_type_t<Bytecode::InstructionType>>(nb);
			}
		};
		
		std::vector<Bytecode::Instruction> m_instructions;
		std::unordered_map<Bytecode::InstructionType, Operation, hash_fn> m_operations;
	};
}
