#pragma once

#include <string>
#include <vector>
#include <sstream>

#include <CppUtils/Type/Concepts.hpp>
#include <CppUtils/Type/Operators.hpp>
#include <CppUtils/Log/Logger.hpp>

namespace CppUtils::Graph
{
	template<typename Storage>
	struct TreeNode
	{
		Storage value;
		std::vector<TreeNode<Storage>> childs;

		TreeNode(Storage c_value, std::vector<TreeNode<Storage>> c_childs = {}):
			value{std::move(c_value)},
			childs{std::move(c_childs)}
		{}

		[[nodiscard]] bool operator==(const TreeNode<Storage>& rhs) const
		{
			return (value == rhs.value && childs == rhs.childs);
		}

		[[nodiscard]] bool exists(const Storage& key) const noexcept
		{
			for (const auto& child : childs)
				if (child.value == key)
					return true;
			return false;
		}

		[[nodiscard]] TreeNode<Storage>& operator[](const Storage& key)
		{
			for (auto& child : childs)
				if (child.value == key)
					return child;
			childs.emplace_back(TreeNode<Storage>{key});
			return childs[childs.size() - 1];
		}

		[[nodiscard]] const TreeNode<Storage>& at(const Storage& key) const
		{
			for (const auto& child : childs)
				if (child.value == key)
					return child;
			throw std::out_of_range{"The TreeNode does not contain the requested child"};
		}
	};

	template<typename Storage>// requires Type::Concept::isPrintable<Storage> // Fonctionne sur GCC mais pas sur Clang https://godbolt.org/z/75cja8
	void logTreeNode(const TreeNode<Storage>& treeNode, const std::string& prefix = " ") noexcept
	{
		auto os = std::ostringstream{};
		os << " " << treeNode.value;
		CppUtils::Log::Logger::logInformation(os.str());
		const auto nbChilds = treeNode.childs.size();

		for (auto i = 0u; i < nbChilds; ++i)
		{
			CppUtils::Log::Logger::logDetail(prefix + ((i != nbChilds - 1) ? "├" : "└") + "─", false);
			logTreeNode(treeNode.childs.at(i), prefix + ((i != nbChilds - 1) ? "│" : " ") + "  ");
		}
	}
}
