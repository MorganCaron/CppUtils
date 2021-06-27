#pragma once

#include <mutex>
#include <memory>
#include <vector>
#include <utility>
#include <functional>

#include <CppUtils/Graph/MeshNode.hpp>

namespace CppUtils::Graph
{
	template<typename Key, typename Value, typename Hash = std::hash<Key>>
	class MeshNetwork
	{
	public:
		using Node = MeshNode<Key, Value, Hash>;

		template<typename... Args>
		[[nodiscard]] inline std::weak_ptr<Node> newNode(Args&&... params)
		{
			const auto lock = std::lock_guard(m_nodesMutex);
			return m_nodes.emplace_back(std::make_shared<Node>(std::forward<Args>(params)...));
		}

	private:
		std::mutex m_nodesMutex;
		std::vector<std::shared_ptr<Node>> m_nodes;
	};
}
