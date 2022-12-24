#pragma once

#include <CppUtils/Hash/Token.hpp>
#include <CppUtils/Graph/Tree.hpp>

namespace CppUtils::Language::Parser
{
	using AstNode = Graph::Tree::Node<Hash::Token>;
	
	struct AstRef;
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

		[[nodiscard]] auto find(Hash::Token token) const noexcept
		{
			return std::find_if(std::cbegin(root.nodes), std::cend(root.nodes), [token](const auto& node) -> bool {
				return node.value == token;
			});
		}
	};
	
	struct AstRef final
	{
		std::reference_wrapper<AstNode> node;
		std::reference_wrapper<Hash::TokenNames> tokenNames;

		explicit AstRef(Ast& ast):
			node{ast.root},
			tokenNames{ast.tokenNames}
		{}

		explicit AstRef(AstNode& astNode, Hash::TokenNames& tokenNames):
			node{astNode},
			tokenNames{tokenNames}
		{}

		// Todo: return std::string_view
		[[nodiscard]] auto getNodeName() const -> std::string
		{
			return Hash::getTokenNameOrValue(node.get().value, tokenNames.get());
		}

		auto log() const -> void
		{
			Graph::Tree::log(node.get(), tokenNames.get());
		}
	};

	struct AstCursor final
	{
		std::reference_wrapper<Ast> root;
		std::reference_wrapper<AstNode> currentNode;
	};
}
