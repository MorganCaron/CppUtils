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
	auto log(const Node<T>& node, const std::string& prefix = "") noexcept -> void
	{
		using namespace Hash::Literals;
		auto logger = Log::Logger{std::cout};
		logger
			<< node.value << '\n';
		const auto nbNodes = node.nodes.size();
		for (auto i = 0u; i < nbNodes; ++i)
		{
			logger
				<< Terminal::TextColor::TextColorEnum::Blue
				<< prefix + ((i != nbNodes - 1) ? "├" : "└") + "─ ";
			log(node.nodes.at(i), prefix + ((i != nbNodes - 1) ? "│" : " ") + "  ");
		}
	}

	auto log(const Node<Hash::Token>& node, const Hash::TokenNames& tokenNames, const std::string& prefix = "") noexcept -> void
	{
		using namespace Hash::Literals;
		auto logger = Log::Logger{std::cout};
		logger
			<< Hash::getTokenNameOrValue(node.value, tokenNames) << '\n';
		const auto nbNodes = node.nodes.size();
		for (auto i = 0u; i < nbNodes; ++i)
		{
			logger
				<< Terminal::TextColor::TextColorEnum::Blue
				<< prefix + ((i != nbNodes - 1) ? "├" : "└") + "─ ";
			log(node.nodes.at(i), tokenNames, prefix + ((i != nbNodes - 1) ? "│" : " ") + "  ");
		}
	}
}
