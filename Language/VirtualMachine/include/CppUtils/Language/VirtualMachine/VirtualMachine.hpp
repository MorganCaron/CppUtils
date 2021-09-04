#pragma once

#include <span>
#include <string>
#include <memory>
#include <functional>

#include <CppUtils/Type/Token.hpp>

namespace CppUtils::Language::VirtualMachine
{
	template<typename Instruction, typename Context>
	class VirtualMachine
	{
	public:
		using Operation = std::function<void(const Instruction&, Context&)>;

		explicit VirtualMachine(std::unordered_map<Type::Token, Operation, Type::Token::hash_fn> operations):
			m_operations{std::move(operations)}
		{}

		void run(const Type::Token& token, const Instruction& instruction, Context& context) const
		{
			try
			{
				const auto& operation = m_operations.find(token);
				if (operation == m_operations.end())
					throw std::runtime_error{"Unknown instruction:\n" + std::string{token.name}};
				operation->second(instruction, context);
			}
			catch (const std::exception& exception)
			{
				throw std::runtime_error{"In the virtual machine:\nIn the " + std::string{token.name} + " operation:\n" + exception.what()};
			}
		}

	private:
		std::unordered_map<Type::Token, Operation, Type::Token::hash_fn> m_operations;
	};
}
