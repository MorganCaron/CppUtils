#pragma once

#include <format>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

#include <CppUtils/HashTable.hpp>
#include <CppUtils/Type/Concept.hpp>

namespace CppUtils::Container::Tree
{
	template<class T>
	struct Node final
	{
		using ValueType = T;

		ValueType value;
		std::vector<Node<ValueType>> nodes = {};

		[[nodiscard]] constexpr auto find(const ValueType& key) const noexcept -> auto
		{
			return std::ranges::find_if(nodes, [&key](const auto& node) -> bool {
				return node.value == key;
			});
		}

		[[nodiscard]] constexpr auto find(const ValueType& key) noexcept -> auto
		{
			return std::ranges::find_if(nodes, [&key](const auto& node) -> bool {
				return node.value == key;
			});
		}

		[[nodiscard]] constexpr auto exists(const ValueType& key) const noexcept -> bool
		{
			return find(key) != std::cend(nodes);
		}

		[[nodiscard]] constexpr auto operator[](const ValueType& key) -> Node<ValueType>&
		{
			for (auto& node : nodes)
				if (key == node.value)
					return node;
			return nodes.emplace_back(Node<T>{key});
		}

		[[nodiscard]] constexpr auto operator[](const ValueType& key) const -> const Node<ValueType>&
		{
			const auto value = find(key);
			if (value == std::cend(nodes))
				throw std::out_of_range{"The Node does not contain the requested child."};
			return *value;
		}

		[[nodiscard]] constexpr auto getNodesWithValue(const ValueType& filterValue) const
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
}

namespace std
{
	template<class T, class CharT>
	requires CppUtils::Type::Concept::Specializes<T, CppUtils::Container::Tree::Node> and
		CppUtils::Type::Concept::Printable<typename T::ValueType>
	struct formatter<T, CharT>: public std::formatter<std::basic_string<CharT>, CharT>
	{
		template<class FormatContext>
		auto format(const T& node, FormatContext& context) const -> decltype(context.out())
		{
			auto&& out = context.out();
			formatNode(node, context);
			return std::format_to(out, "\n");
		}

	private:
		template<class FormatContext>
		auto formatNode(const T& node, FormatContext& context, std::string&& prefix = "") const -> void
		{
			auto&& out = context.out();
			std::format_to(out, "{}\n", node.value);
			const auto nbNodes = std::size(node.nodes);
			for (auto i = 0uz; i < nbNodes; ++i)
			{
				std::format_to(out, "{}{}─ ", prefix, (i != nbNodes - 1) ? "├" : "└");
				formatNode(node.nodes.at(i), context, prefix + ((i != nbNodes - 1) ? "│" : " ") + "  ");
			}
		}
	};
}
