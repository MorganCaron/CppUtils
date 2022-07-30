#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Graph::Tree
{
	TEST_GROUP("Graph/Tree")
	{
		using StringTreeNode = CppUtils::Graph::Tree::Node<std::string>;

		addTest("", [] {
			auto root = StringTreeNode{"Root"};
			CppUtils::Graph::Tree::log(root);
			CppUtils::Log::Logger::logInformation(root.value);
			TEST_ASSERT(root.value == "Root");
			
			root.nodes.emplace_back(StringTreeNode{"Branch0"});
			root.nodes.emplace_back(StringTreeNode{"Branch1"});
			CppUtils::Log::Logger::logInformation(root.nodes[1].value);
			TEST_ASSERT(root.nodes[1].value == "Branch1");

			auto& branch0 = root.nodes[0];
			branch0.nodes.emplace_back(StringTreeNode{"SubBranch0"});
			branch0.nodes[0].nodes.emplace_back(StringTreeNode{"SubSubBranch0"});
			branch0.nodes.emplace_back(StringTreeNode{"SubBranch1"});
			branch0.nodes.emplace_back(StringTreeNode{"SubBranch2"});
			TEST_ASSERT(branch0.nodes.size() == 3);

			root.nodes.emplace_back(StringTreeNode{"Branch2"});
			auto& branch2 = root.nodes[2];
			branch2.nodes.emplace_back(StringTreeNode{"SubBranch0"});
			auto& subBranch0 = branch2.nodes[0];
			subBranch0.nodes.emplace_back(StringTreeNode{"SubSubBranch0"});
			subBranch0.nodes.emplace_back(StringTreeNode{"SubSubBranch1"});
			root.nodes.emplace_back(StringTreeNode{"Branch3"});
			CppUtils::Graph::Tree::log(root);
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
			// TEST_ASSERT(jsonTree0 != jsonTree1);
		});*/
	}
}
