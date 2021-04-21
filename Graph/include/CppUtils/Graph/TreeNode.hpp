#pragma once

#include <string>
#include <vector>
#include <sstream>

#include <CppUtils/Type/Traits.hpp>
#include <CppUtils/Type/Variant.hpp>
#include <CppUtils/Log/Logger.hpp>

namespace CppUtils::Graph
{
	template<typename Storage>
	struct TreeNode
	{
		Storage value;
		std::vector<TreeNode<Storage>> childs;

		explicit TreeNode(Storage c_value, std::vector<TreeNode<Storage>> c_childs = {}):
			value{std::move(c_value)},
			childs{std::move(c_childs)}
		{}

		template<typename RhsStorage>
		[[nodiscard]] bool operator==(const TreeNode<RhsStorage>& rhs) const
		{
			if constexpr(std::same_as<Storage, RhsStorage>)
				return (value == rhs.value && childs == rhs.childs);
			else
			{
				const auto nbChilds = childs.size();
				if (value != rhs.value || nbChilds != rhs.childs.size())
					return false;
				for (auto i = std::size_t{0}; i < nbChilds; ++i)
					if (childs.at(i) != rhs.childs.at(i))
						return false;
				return true;
			}
		}

		[[nodiscard]] bool exists(const Storage& key) const noexcept
		{
			for (const auto& child : childs)
				if (key == child.value)
					return true;
			return false;
		}

		[[nodiscard]] TreeNode<Storage>& operator[](const Storage& key)
		{
			for (auto& child : childs)
				if (key == child.value)
					return child;
			childs.emplace_back(TreeNode<Storage>{key});
			return childs[childs.size() - 1];
		}

		[[nodiscard]] const TreeNode<Storage>& at(const Storage& key) const
		{
			for (const auto& child : childs)
				if (key == child.value)
					return child;
			throw std::out_of_range{"The TreeNode does not contain the requested child."};
		}

		[[nodiscard]] inline const Storage& getChildValue(std::size_t nb = 0) const
		{
			return childs.at(nb).value;
		}

		void forEach(const Storage& key, const std::function<void(const TreeNode<Storage>&)>& function) const
		{
			if (key == value)
				function(*this);
			for (const auto& child : childs)
				child.forEach(key, function);
		}

		void forEach(const Storage& key, const std::function<void(TreeNode<Storage>&)>& function)
		{
			if (key == value)
				function(*this);
			for (auto& child : childs)
				child.forEach(key, function);
		}
	};

	template<typename Storage>// requires Type::Traits::isPrintable<Storage> // Fonctionne sur GCC mais pas sur Clang https://godbolt.org/z/75cja8
	void logTreeNode(const TreeNode<Storage>& treeNode, const std::string& prefix = " ") noexcept
	{
		auto os = std::ostringstream{};
		os << " " << treeNode.value;
		Log::Logger::logInformation(os.str());
		const auto nbChilds = treeNode.childs.size();

		for (auto i = 0u; i < nbChilds; ++i)
		{
			Log::Logger::logDetail(prefix + ((i != nbChilds - 1) ? "├" : "└") + "─", false);
			logTreeNode(treeNode.childs.at(i), prefix + ((i != nbChilds - 1) ? "│" : " ") + "  ");
		}
	}
}
