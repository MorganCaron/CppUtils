#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Graph::TreeNode
{
	TEST_GROUP("Graph/TreeNode")
	{
		using namespace CppUtils::Language::Json::Literals;
		
		using StringTreeNode = CppUtils::Graph::TreeNode<std::string>;

		addTest("", [] {
			auto root = StringTreeNode{"Root"};
			CppUtils::Log::Logger::logInformation(root.value);
			ASSERT(root.value == "Root");
			
			root.childs.emplace_back(StringTreeNode{"Branch0"});
			root.childs.emplace_back(StringTreeNode{"Branch1"});
			CppUtils::Log::Logger::logInformation(root.getChildValue(1));
			ASSERT(root.getChildValue(1) == "Branch1");

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
			CppUtils::Log::TreeNodeLogger::log(root);
		});

		/*addTest("Differences", [] {
			[[maybe_unused]] const auto jsonTree0 = R"(
			{
				"removed": "value",
				"branch0": {
					"key0": "value0",
					"key1": "value1"
				}
			}
			)"_json;
			[[maybe_unused]] const auto jsonTree1 = R"(
			{
				"branch0": {
					"key0": "value0",
					"key1": "edited"
				},
				"added": "value"
			}
			)"_json;
			// const auto differences = 
			// ASSERT(jsonTree0 != jsonTree1);
		});*/
	}
}
