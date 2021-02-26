#pragma once

#include <functional>

#include <CppUtils/Language/Parser/Cursor.hpp>
#include <CppUtils/Graph/VariantTreeNode.hpp>

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
