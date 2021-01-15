#pragma once

#include <functional>

#include <CppUtils/Language/Parser/Cursor.hpp>
#include <CppUtils/Graph/VariantTreeNode.hpp>

namespace CppUtils::Language::Parser
{
	template<typename... Types>
	struct Context final
	{
		Parser::Cursor<std::string> cursor;
		std::reference_wrapper<Graph::VariantTreeNode<Types...>> parentNode;
	};

	template<typename... Types>
	using ParsingFunction = std::function<bool (Context<Types...>&)>;
}
