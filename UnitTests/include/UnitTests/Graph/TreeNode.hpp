#pragma once

#include <CppUtils.hpp>

namespace UnitTests::Graph::TreeNode
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("Graph/TreeNode", [] {
			using StringTreeNode = CppUtils::Graph::TreeNode<const std::string>;
			auto root = StringTreeNode{"Root"};
			root.childs.emplace_back(StringTreeNode{"Branch0"});
			root.childs.emplace_back(StringTreeNode{"Branch1"});
			CppUtils::Log::Logger::logInformation(root.self);
			ASSERT(root.self == "Root");
			CppUtils::Log::Logger::logInformation(root.childs.at(1).self);
			ASSERT(root.childs.at(1).self == "Branch1");
		})

	};
}
