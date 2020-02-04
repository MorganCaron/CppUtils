#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std::string_literals;

namespace CppUtils
{
	template<typename Value>
	class WebNode
	{
	public:
		using Link = std::shared_ptr<WebNode<Value>>;

		explicit WebNode(const Value& m_value)
			: value(m_value)
		{};

		inline bool exists(std::string_view branchName) const
		{
			return m_branchs.find(branchName.data()) != m_branchs.end();
		};

		std::vector<Link> get(std::string_view branchName) const
		{
			if (!exists(branchName))
				throw std::out_of_range("La branche "s + branchName.data() + " n'existe pas dans le WebNode.");
			return m_branchs.at(branchName.data());
		};

		std::vector<Link>& operator[](std::string_view branchName)
		{
			if (!exists(branchName))
				m_branchs[branchName.data()] = std::vector<Link>();
			return m_branchs[branchName.data()];
		};

		void attach(std::string_view branchName, const Link& node)
		{
			if (!exists(branchName))
				m_branchs[branchName.data()] = std::vector<Link>();
			m_branchs[branchName.data()].push_back(node);
		};

		inline void detach(std::string_view branchName)
		{
			m_branchs.erase(branchName);
		};

		Value value;

	protected:
		std::unordered_map<std::string, std::vector<Link>> m_branchs;
	};
}
