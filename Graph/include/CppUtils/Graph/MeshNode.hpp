#pragma once

#include <memory>
#include <vector>
#include <stdexcept>
#include <functional>
#include <unordered_map>

namespace CppUtils::Graph
{
	template<typename Key, typename Value, typename Hash = std::hash<Key>>
	class MeshNode
	{
	public:
		using Node = MeshNode<Key, Value, Hash>;

		static inline void attach(const Key& branchKey0, const std::shared_ptr<Node>& node0, const Key& branchKey1, const std::shared_ptr<Node>& node1)
		{
			node1->attach(branchKey0, node0);
			node0->attach(branchKey1, node1);
		}
		
		explicit MeshNode(Value c_value):
			value(std::move(c_value))
		{}

		[[nodiscard]] inline bool exists(const Key& branchKey) const
		{
			return m_branchs.find(branchKey) != m_branchs.end();
		}

		[[nodiscard]] const std::vector<std::weak_ptr<Node>>& get(const Key& branchKey) const
		{
			if (!exists(branchKey))
				throw std::out_of_range("The branch does not exist in the MeshNode.");
			return m_branchs.at(branchKey);
		}

		[[nodiscard]] std::vector<std::weak_ptr<Node>>& operator[](const Key& branchKey)
		{
			if (!exists(branchKey))
				m_branchs[branchKey] = std::vector<std::weak_ptr<Node>>{};
			return m_branchs[branchKey];
		}

		void attach(const Key& branchKey, const std::shared_ptr<Node>& node)
		{
			if (!exists(branchKey))
				m_branchs[branchKey] = std::vector<std::weak_ptr<Node>>{};
			m_branchs[branchKey].push_back(node);
		}

		inline void detach(const Key& branchKey)
		{
			m_branchs.erase(branchKey);
		}

		[[nodiscard]] inline std::size_t count(const Key& branchKey) const
		{
			return exists(branchKey) ? static_cast<std::size_t>(get(branchKey).size()) : 0;
		}

		Value value;

	protected:
		std::unordered_map<Key, std::vector<std::weak_ptr<Node>>, Hash> m_branchs;
	};
}
