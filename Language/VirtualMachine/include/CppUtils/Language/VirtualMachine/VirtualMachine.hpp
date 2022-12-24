#pragma once

#include <span>
#include <string>
#include <memory>
#include <functional>

#include <CppUtils/Hash/Token.hpp>

namespace CppUtils::Language::VirtualMachine
{
	/*
	template<class... Args>
	class VirtualMachine final
	{
	public:
		using Operation = std::function<bool(Args&&...)>;

		auto addOperation(std::string_view tokenName, Operation operation) -> void
		{
			auto token = Hash::hash(tokenName);
			m_operations[token] = std::move(operation);
			m_tokenNames[token] = tokenName;
		}

		auto run(const Hash::Token& token, Args&&... args) const -> bool
		{
			try
			{
				const auto& operation = m_operations.find(token);
				if (operation == std::cend(m_operations))
					throw std::runtime_error{"Unknown instruction:\n" + Hash::getTokenNameOrValue(token, m_tokenNames)};
				return operation->second(std::forward<Args>(args)...);
			}
			catch (const std::exception& exception)
			{
				throw std::runtime_error{"In the virtual machine:\nIn the " + Hash::getTokenNameOrValue(token, m_tokenNames) + " operation:\n" + exception.what()};
			}
			return false;
		}

	private:
		std::unordered_map<Hash::Token, Operation> m_operations;
		Hash::TokenNames m_tokenNames;
	};
	*/
}
