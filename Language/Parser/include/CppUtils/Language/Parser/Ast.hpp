#pragma once

#include <CppUtils/Hash/Token.hpp>
#include <CppUtils/Graph/Tree.hpp>

namespace CppUtils::Language::Parser
{
	using AstNode = Graph::Tree::Node<Hash::Token>;

	struct Ast final
	{
		AstNode root;
		Hash::TokenNames tokenNames;

		explicit Ast(Hash::TokenNames c_tokenNames = {})
		{
			using namespace Hash::Literals;
			root = AstNode{"root"_token};
			tokenNames = std::move(c_tokenNames);
			tokenNames["root"_token] = "root";
		}

		auto log() const -> void
		{
			Graph::Tree::log(root, tokenNames);
		}
	};
	
	struct ReadableAstNode final
	{
		std::reference_wrapper<AstNode> node;
		std::reference_wrapper<Hash::TokenNames> tokenNames;

		explicit ReadableAstNode(Ast& ast):
			node{ast.root},
			tokenNames{ast.tokenNames}
		{}

		ReadableAstNode(AstNode& astNode, Hash::TokenNames& tokenNames):
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
}
