#pragma once

#include <string>
#include <vector>
#include <sstream>

#include <CppUtils/Type/Traits.hpp>
#include <CppUtils/Type/Variant.hpp>
#include <CppUtils/Log/Logger.hpp>
#include <CppUtils/Container/Sequence.hpp>

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
					if (childs[i] != rhs.childs[i])
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
			return childs.emplace_back(TreeNode<Storage>{key});
		}

		[[nodiscard]] const TreeNode<Storage>& at(const Storage& key) const
		{
			for (const auto& child : childs)
				if (key == child.value)
					return child;
			throw std::out_of_range{"The TreeNode does not contain the requested child."};
		}

		[[nodiscard]] const Storage& getChildValue(std::size_t nb = 0) const
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
}
