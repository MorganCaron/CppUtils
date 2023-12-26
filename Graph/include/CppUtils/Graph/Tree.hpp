#pragma once

#include <string>
#include <ranges>
#include <vector>
#include <iostream>

#include <CppUtils/Log/Logger.hpp>
#include <CppUtils/HashTable.hpp>
#include <CppUtils/Type/Concept.hpp>

namespace CppUtils::Graph::Tree
{
	template<class T>
	struct Node final
	{
		T value;
		std::vector<Node<T>> nodes = {};

		[[nodiscard]] constexpr auto find(const T& key) const noexcept -> auto
		{
			return std::ranges::find_if(nodes, [&key](const auto& node) -> bool {
				return node.value == key;
			});
		}

		[[nodiscard]] constexpr auto find(const T& key) noexcept -> auto
		{
			return std::ranges::find_if(nodes, [&key](const auto& node) -> bool {
				return node.value == key;
			});
		}

		[[nodiscard]] constexpr auto exists(const T& key) const noexcept -> bool
		{
			return find(key) != std::cend(nodes);
		}

		[[nodiscard]] constexpr auto operator[](const T& key) -> Node<T>&
		{
			for (auto& node : nodes)
				if (key == node.value)
					return node;
			return nodes.emplace_back(Node<T>{key});
		}

		[[nodiscard]] constexpr auto operator[](const T& key) const -> const Node<T>&
		{
			const auto value = find(key);
			if (value == std::cend(nodes))
				throw std::out_of_range{"The Node does not contain the requested child."};
			return *value;
		}
		
		[[nodiscard]] constexpr auto getNodesWithValue(const T& filterValue) const
		{
			return nodes | std::views::filter([&filterValue](const auto& node) { return node.value == filterValue; });
		}
	};

	template<class T>
	Node(T value, std::vector<Node<T>> nodes = {}) -> Node<T>;

	template<class T>
	[[nodiscard]] auto operator==(const Node<T>& lhs, const Node<T>& rhs) -> bool
	{
		return lhs.value == rhs.value && lhs.nodes == rhs.nodes;
	}

	template<class T, class Logger = Logger<"CppUtils">>
	requires Type::Concept::Printable<T> // Fonctionne sur GCC mais pas sur Clang https://godbolt.org/z/75cja8
	auto log(const Node<T>& node, const std::string& prefix = "") noexcept -> void
	{
		Logger::print("{}\n", node.value);
		const auto nbNodes = std::size(node.nodes);
		for (auto i = 0uz; i < nbNodes; ++i)
		{
			Logger::print("{}{}─ ", prefix, (i != nbNodes - 1) ? "├" : "└");
			log<T, Logger>(node.nodes.at(i), prefix + ((i != nbNodes - 1) ? "│" : " ") + "  ");
		}
	}

	template<class Logger = Logger<"CppUtils">>
	auto log(const Node<Token>& node, const HashTable::TokenNames& tokenNames, const std::string& prefix = "") noexcept -> void
	{
		Logger::print("{}\n", HashTable::getTokenNameOrValue(node.value, tokenNames));
		const auto nbNodes = std::size(node.nodes);
		for (auto i = 0uz; i < nbNodes; ++i)
		{
			Logger::print("{}{}─ ", prefix, (i != nbNodes - 1) ? "├" : "└");
			log(node.nodes.at(i), tokenNames, prefix + ((i != nbNodes - 1) ? "│" : " ") + "  ");
		}
	}
}
