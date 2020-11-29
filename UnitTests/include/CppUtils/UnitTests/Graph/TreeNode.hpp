#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Graph::TreeNode
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest{"Graph/TreeNode", [] {
			using StringTreeNode = CppUtils::Graph::TreeNode<std::string>;
			
			auto root = StringTreeNode{"Root"};
			CppUtils::Log::Logger::logInformation(root.self);
			ASSERT(root.self == "Root");
			
			root.childs.emplace_back(StringTreeNode{"Branch0"});
			root.childs.emplace_back(StringTreeNode{"Branch1"});
			CppUtils::Log::Logger::logInformation(root.childs.at(1).self);
			ASSERT(root.childs.at(1).self == "Branch1");

			auto& branch0 = root.childs[0];
			branch0.childs.emplace_back(StringTreeNode{"SubBranch0"});
			branch0.childs[0].childs.emplace_back(StringTreeNode{"SubSubBranch0"});
			branch0.childs.emplace_back(StringTreeNode{"SubBranch1"});
			branch0.childs.emplace_back(StringTreeNode{"SubBranch2"});
			ASSERT(branch0.childs.size() == 3);

			root.childs.emplace_back(StringTreeNode{"Branch2"});
			auto& branch2 = root.childs[2];
			branch2.childs.emplace_back(StringTreeNode{"SubBranch0"});
			auto& subBranch0 = branch2.childs[0];
			subBranch0.childs.emplace_back(StringTreeNode{"SubSubBranch0"});
			subBranch0.childs.emplace_back(StringTreeNode{"SubSubBranch1"});
			root.childs.emplace_back(StringTreeNode{"Branch3"});
			CppUtils::Terminal::setConsoleOutputUTF8();
			CppUtils::Graph::logTreeNode(root);
		}}

	};
}
