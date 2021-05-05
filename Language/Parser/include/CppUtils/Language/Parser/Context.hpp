#pragma once

#include <stack>
#include <functional>

#include <CppUtils/Graph/VariantTreeNode.hpp>
#include <CppUtils/Language/Parser/Cursor.hpp>

namespace CppUtils::Language::Parser
{
	template<typename... Types>
	using ASTNode = Graph::VariantTreeNode<Types...>;

	template<typename... Types>
	struct Context final
	{
		Parser::Cursor<std::string> cursor;
		std::reference_wrapper<ASTNode<Types...>> parentNode;
	};

	template<typename... Types>
	using ParsingFunction = std::function<bool (Context<Types...>&)>;
}
