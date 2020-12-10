#pragma once

#include <memory>
#include <vector>
#include <stdexcept>
#include <unordered_map>

namespace CppUtils::Graph
{
	template<typename Key, typename Value>
	class MeshNode
	{
	public:
		using Link = std::shared_ptr<MeshNode<Key, Value>>;

		static inline void attach(const Key& branchName0, const Link& node0, const Key& branchName1, const Link& node1)
		{
			node1->attach(branchName0, node0);
			node0->attach(branchName1, node1);
		}
		
		explicit MeshNode(Value c_value): value(std::move(c_value))
		{}

		[[nodiscard]] inline bool exists(const Key& branchName) const
		{
			return m_branchs.find(branchName.data()) != m_branchs.end();
		}

		[[nodiscard]] const std::vector<Link>& get(const Key& branchName) const
		{
			if (!exists(branchName))
				throw std::out_of_range("La branche n'existe pas dans le MeshNode.");
			return m_branchs.at(branchName.data());
		}

		[[nodiscard]] std::vector<Link>& operator[](const Key& branchName)
		{
			if (!exists(branchName))
				m_branchs[branchName.data()] = std::vector<Link>{};
			return m_branchs[branchName.data()];
		}

		void attach(const Key& branchName, const Link& node)
		{
			if (!exists(branchName))
				m_branchs[branchName.data()] = std::vector<Link>{};
			m_branchs[branchName.data()].push_back(node);
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
		std::unordered_map<Key, std::vector<Link>> m_branchs;
	};
}
