#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include <CppUtils/Graph/MeshNode.hpp>

namespace CppUtils::Graph
{
	template<class Key, class Value, class Hash = std::hash<Key>>
	class MeshNetwork
	{
	public:
		using Node = MeshNode<Key, Value, Hash>;

		template<class... Args>
		[[nodiscard]] std::shared_ptr<Node> newNode(Args&&... params)
		{
			[[maybe_unused]] const auto lock = std::lock_guard(m_nodesMutex);
			return m_nodes.emplace_back(std::make_shared<Node>(std::forward<Args>(params)...));
		}

	protected:
		std::mutex m_nodesMutex;
		std::vector<std::shared_ptr<Node>> m_nodes;
	};

	template<bool doubleLinked, class Key, class Value, class Hash = std::hash<Key>>
	requires std::is_integral_v<Key>
	class FragmentedMeshNetwork: public MeshNetwork<Key, Value, Hash>
	{
	public:
		using Node = MeshNode<Key, Value, Hash>;

		enum Branch : Key
		{
			Last,
			Next
		};

		template<class Type>
		[[nodiscard]] std::shared_ptr<Node> newChain(Type data)
		{
			static_assert(sizeof(Type) >= sizeof(Value));
			static_assert(sizeof(Type) % sizeof(Value) == 0);
			auto firstNode = this->newNode(*reinterpret_cast<Value*>(&data)), lastNode = firstNode;
			for (std::size_t i = 1u; i * sizeof(Value) < sizeof(Type); ++i)
			{
				auto nextNode = this->newNode(*reinterpret_cast<Value*>(&data + i * sizeof(Value)));
				(doubleLinked ? Node::attach(Last, lastNode, Next, nextNode) : lastNode->attach(Next, nextNode));
				lastNode = nextNode;
			}
			return firstNode;
		}

		template<class Type>
		[[nodiscard]] std::shared_ptr<Node> getBranchOrDefault(const std::shared_ptr<Node>& baseNode, Key branchKey, Type&& defaultValue)
		{
			if (!baseNode->exists(branchKey))
				baseNode->attach(branchKey, newChain(std::move(defaultValue)));
			return std::shared_ptr<Node>{(*baseNode)[branchKey][0]};
		}

		template<class Type>
		[[nodiscard]] std::shared_ptr<Node> getBranchOrDefault(Key baseKey, const std::shared_ptr<Node>& baseNode, Key branchKey, Type&& defaultValue)
		{
			if (!baseNode->exists(branchKey))
				Node::attach(baseKey, baseNode, branchKey, newChain(std::move(defaultValue)));
			return std::shared_ptr<Node>{(*baseNode)[branchKey][0]};
		}
	};
}
