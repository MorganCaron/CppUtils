#pragma once

#include <string>
#include <ranges>
#include <vector>
#include <iostream>

#include <CppUtils/Log/Logger.hpp>
#include <CppUtils/Hash/Token.hpp>
#include <CppUtils/Type/Concept.hpp>

namespace CppUtils::Graph::Tree
{
	template<class T>
	struct Node final
	{
		T value;
		std::vector<Node<T>> nodes = {};

		[[nodiscard]] auto exists(const T& key) const noexcept -> bool
		{
			for (const auto& node : nodes)
				if (key == node.value)
					return true;
			return false;
		}

		[[nodiscard]] auto operator[](const T& key) -> Node<T>&
		{
			for (auto& node : nodes)
				if (key == node.value)
					return node;
			return nodes.emplace_back(Node<T>{key});
		}

		[[nodiscard]] auto operator[](const T& key) const -> const Node<T>&
		{
			for (const auto& node : nodes)
				if (key == node.value)
					return node;
			throw std::out_of_range{"The Node does not contain the requested child."};
		}
		
		[[nodiscard]] auto getNodesWithValue(const T& filterValue) const
		{
			return nodes | std::views::filter([&filterValue](const auto& node) { return node.value == filterValue; });
		}
	};

	template<class T>
	Node(T value, std::vector<Node<T>> nodes = {}) -> Node<T>;

	template<class T>
	requires Type::Concept::Printable<T> // Fonctionne sur GCC mais pas sur Clang https://godbolt.org/z/75cja8
	void log(const Node<T>& node, const std::string& prefix = "") noexcept
	{
		using namespace Hash::Literals;
		auto os = std::ostringstream{};
		os << node.value;
		Log::Logger::logInformation(os.str());
		const auto nbNodes = node.nodes.size();
		for (auto i = 0u; i < nbNodes; ++i)
		{
			Log::Logger::log("Information"_token, prefix + ((i != nbNodes - 1) ? "├" : "└") + "─ ", Terminal::TextColor::TextColorEnum::Blue, false);
			log(node.nodes.at(i), prefix + ((i != nbNodes - 1) ? "│" : " ") + "  ");
		}
	}

	void log(const Node<Hash::Token>& node, const Hash::TokenNames& tokenNames, const std::string& prefix = "") noexcept
	{
		using namespace Hash::Literals;
		auto os = std::ostringstream{};
		os << Hash::getTokenNameOrValue(node.value, tokenNames);
		Log::Logger::logInformation(os.str());
		const auto nbNodes = node.nodes.size();
		for (auto i = 0u; i < nbNodes; ++i)
		{
			Log::Logger::log("Information"_token, prefix + ((i != nbNodes - 1) ? "├" : "└") + "─ ", Terminal::TextColor::TextColorEnum::Blue, false);
			log(node.nodes.at(i), tokenNames, prefix + ((i != nbNodes - 1) ? "│" : " ") + "  ");
		}
	}
}
