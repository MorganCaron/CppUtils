#pragma once

#include <CppUtils/Hash/Token.hpp>
#include <CppUtils/Graph/Tree.hpp>

namespace CppUtils::Language
{
	using AstNode = Graph::Tree::Node<Hash::Token>;
	
	struct Ast final
	{
		AstNode root;
		Hash::TokenNames tokenNames;

		explicit Ast(std::string astName, std::vector<AstNode> declarations = {}, Hash::TokenNames c_tokenNames = {})
		{
			using namespace Hash::Literals;
			const auto token = Hash::hash(astName);
			root = AstNode{token, std::move(declarations)};
			tokenNames = std::move(c_tokenNames);
			tokenNames[token] = std::move(astName);
		}

		auto log() const -> void
		{
			Graph::Tree::log(root, tokenNames);
		}

		[[nodiscard]] auto find(Hash::Token token) const noexcept -> decltype(root.nodes)::const_iterator
		{
			return std::find_if(std::cbegin(root.nodes), std::cend(root.nodes), [token](const auto& node) -> bool {
				return node.value == token;
			});
		}
	};
}
