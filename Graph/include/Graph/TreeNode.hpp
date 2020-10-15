#pragma once

#include <string>
#include <vector>
#include <sstream>

#include <Log/Logger.hpp>

namespace CppUtils::Graph
{
	template<typename Storage>
	struct TreeNode
	{
		Storage self;
		std::vector<TreeNode<Storage>> childs = {};
	};

	template<typename Storage>
	void logTreeNode(const TreeNode<Storage>& treeNode, const std::string& prefix = " ") noexcept
	{
		auto os = std::ostringstream{};
		os << " " << treeNode.self;
		CppUtils::Log::Logger::logInformation(os.str());
		const auto nbChilds = treeNode.childs.size();

		for (auto i = 0u; i < nbChilds; ++i)
		{
			CppUtils::Log::Logger::logDetail(prefix + ((i != nbChilds - 1) ? "├" : "└") + "─", false);
			logTreeNode(treeNode.childs.at(i), prefix + ((i != nbChilds - 1) ? "|" : " ") + "  ");
		}
	}
	/*
	├ = 195
	─ = 196
	│ = 179
	└ = 192
	*/
}
