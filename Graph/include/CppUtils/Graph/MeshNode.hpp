#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <unordered_map>

namespace CppUtils::Graph
{
	template<typename Key, typename Value, typename Hash = std::hash<Key>>
	class MeshNode
	{
	public:
		using Link = std::weak_ptr<MeshNode<Key, Value, Hash>>;

		static inline void attach(const Key& branchName0, const Link& link0, const Key& branchName1, const Link& link1, bool unique = false)
		{
			if (auto node0 = link0.lock(), node1 = link1.lock(); node0 && node1)
			{
				node1->attach(branchName0, link0, unique);
				node0->attach(branchName1, link1, unique);
			}
		}
		
		explicit MeshNode(Value c_value): value(std::move(c_value))
		{}

		[[nodiscard]] inline bool exists(const Key& branchName) const
		{
			return m_branchs.find(branchName) != m_branchs.end();
		}

		[[nodiscard]] const std::vector<Link>& get(const Key& branchName) const
		{
			if (!exists(branchName))
				throw std::out_of_range("The branch does not exist in the MeshNode.");
			return m_branchs.at(branchName);
		}

		[[nodiscard]] std::vector<Link>& operator[](const Key& branchName)
		{
			if (!exists(branchName))
				m_branchs[branchName] = std::vector<Link>{};
			return m_branchs[branchName];
		}

		void attach(const Key& branchName, const Link& link, bool unique = false)
		{
			if (!exists(branchName))
				m_branchs[branchName] = std::vector<Link>{};
			if (unique)
			{
				const auto node = link.lock();
				const auto& links = m_branchs.at(branchName);
				if (!node || std::find_if(links.begin(), links.end(), [&node](const auto& storedLink) {
					if (const auto storedNode = storedLink.lock())
						return node->value == storedNode->value;
					else
						return false;
				}) != links.end())
					return;
			}
			m_branchs[branchName].push_back(link);
		}

		inline void detach(const Key& branchName)
		{
			m_branchs.erase(branchName);
		}

		[[nodiscard]] inline std::size_t count(const Key& branchName) const
		{
			return exists(branchName) ? static_cast<std::size_t>(get(branchName).size()) : 0;
		}

		Value value;

	protected:
		std::unordered_map<Key, std::vector<Link>, Hash> m_branchs;
	};
}
