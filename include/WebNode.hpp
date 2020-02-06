#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

namespace CppUtils
{
	template<typename Key, typename Value>
	class WebNode
	{
	public:
		using Link = std::shared_ptr<WebNode<Key, Value>>;

		explicit WebNode(const Value& m_value)
			: value(m_value)
		{}

		inline bool exists(const Key& branchName) const
		{
			return m_branchs.find(branchName.data()) != m_branchs.end();
		}

		std::vector<Link> get(const Key& branchName) const
		{
			if (!exists(branchName))
				throw std::out_of_range("La branche n'existe pas dans le WebNode.");
			return m_branchs.at(branchName.data());
		}

		std::vector<Link>& operator[](const Key& branchName)
		{
			if (!exists(branchName))
				m_branchs[branchName.data()] = std::vector<Link>();
			return m_branchs[branchName.data()];
		}

		void attach(const Key& branchName, const Link& node)
		{
			if (!exists(branchName))
				m_branchs[branchName.data()] = std::vector<Link>();
			m_branchs[branchName.data()].push_back(node);
		}

		inline void detach(const Key& branchName)
		{
			m_branchs.erase(branchName);
		}

		Value value;

	protected:
		std::unordered_map<Key, std::vector<Link>> m_branchs;
	};
}
