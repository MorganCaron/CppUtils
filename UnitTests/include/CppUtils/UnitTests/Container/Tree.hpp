#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::Container::Tree
{
	auto _ = TestSuite{"Container/Tree", [](auto& suite) {
		using Logger = CppUtils::Logger<"CppUtils">;
		using StringTreeNode = CppUtils::Container::Tree::Node<std::string>;

		suite.addTest("", [&] {
			auto root = StringTreeNode{"Root"};
			// CppUtils::Container::Tree::log(root);
			Logger::print("{}\n", root.value);
			suite.expect(root.value == "Root");

			root.nodes.emplace_back(StringTreeNode{"Branch0"});
			root.nodes.emplace_back(StringTreeNode{"Branch1"});
			Logger::print("{}\n", root.nodes[1].value);
			suite.expect(root.nodes[1].value == "Branch1");

			auto& branch0 = root.nodes[0];
			branch0.nodes.emplace_back(StringTreeNode{"SubBranch0"});
			branch0.nodes[0].nodes.emplace_back(StringTreeNode{"SubSubBranch0"});
			branch0.nodes.emplace_back(StringTreeNode{"SubBranch1"});
			branch0.nodes.emplace_back(StringTreeNode{"SubBranch2"});
			suite.expect(branch0.nodes.size() == 3);

			root.nodes.emplace_back(StringTreeNode{"Branch2"});
			auto& branch2 = root.nodes[2];
			branch2.nodes.emplace_back(StringTreeNode{"SubBranch0"});
			auto& subBranch0 = branch2.nodes[0];
			subBranch0.nodes.emplace_back(StringTreeNode{"SubSubBranch0"});
			subBranch0.nodes.emplace_back(StringTreeNode{"SubSubBranch1"});
			root.nodes.emplace_back(StringTreeNode{"Branch3"});

			std::print("{}", root);
		});

		/*suite.addTest("Differences", [&] {
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
			// suite.expect(jsonTree0 != jsonTree1);
		});*/
	}};
}
