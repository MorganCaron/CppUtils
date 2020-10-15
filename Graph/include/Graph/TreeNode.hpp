#pragma once

#include <vector>

namespace CppUtils::Graph
{
	template<typename Storage>
	struct TreeNode
	{
		Storage self;
		std::vector<TreeNode<Storage>> childs = {};
	};
}
