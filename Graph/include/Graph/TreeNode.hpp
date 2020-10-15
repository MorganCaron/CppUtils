#pragma once

#include <string>
#include <vector>

namespace CppUtils::Graph
{
	template<typename Storage>
	struct TreeNode
	{
		Storage self;
		std::vector<TreeNode<Storage>> childs = {};
	};

	std::string treeNodeToString(const TreeNode<std::string> treeNode, const std::string prefix = "") noexcept
	{
		auto output = " " + treeNode.self + '\n';
		const auto nbChilds = treeNode.childs.size();

		for (auto i = 0u; i < nbChilds; ++i)
			output += prefix + " " + ((i != nbChilds - 1) ? "├" : "└") + "─" + treeNodeToString(treeNode.childs.at(i), prefix + ((i != nbChilds - 1) ? " |" : "  "));
		return output;
	}
	/*
	├ = 195
	─ = 196
	│ = 179
	└ = 192
	*/

}
