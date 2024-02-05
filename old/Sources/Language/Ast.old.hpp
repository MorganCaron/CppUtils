#pragma once

#include <CppUtils/Container/Tree.hpp>
#include <CppUtils/String/Hash.hpp>

namespace CppUtils::Language
{
	using AstNode = Container::Tree::Node<String::Token>;

	struct Ast final
	{
		AstNode root;
		String::HashTable tokenNames;

		explicit Ast(std::string astName, std::vector<AstNode> declarations = {}, String::HashTable c_tokenNames = {})
		{
			using namespace String::Literals;
			const auto token = String::hash(astName);
			root = AstNode{token, std::move(declarations)};
			tokenNames = std::move(c_tokenNames);
			tokenNames[token] = std::move(astName);
		}

		[[nodiscard]] auto find(String::Token token) const noexcept -> decltype(root.nodes)::const_iterator
		{
			return std::find_if(std::cbegin(root.nodes), std::cend(root.nodes), [token](const auto& node) -> bool {
				return node.value == token;
			});
		}

		[[nodiscard]] inline auto getStackNode(std::size_t argumentNumber) -> AstNode&
		{
			using namespace String::Literals;
			auto& stack = root["stack"_token].nodes;
			return stack[std::size(stack) - 1 - argumentNumber];
		}

		[[nodiscard]] static inline auto getFirstNode(AstNode& node) -> AstNode&
		{
			return std::empty(node.nodes) ? node.nodes.emplace_back() : node.nodes.front();
		}

		static inline auto pushPointer(AstNode& contextNode, const auto* data) -> void
		{
			contextNode.nodes.emplace_back(reinterpret_cast<String::Token>(data));
		}
	};
}
