#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::StringTree::StringTreeLexer
{
	TEST_GROUP("Language/StringTree/StringTreeLexer")
	{
		using namespace std::literals;
		using namespace CppUtils::Language::StringTree::Literals;
		
		addTest("parse", [] {
			const auto stringTreeLexer = CppUtils::Language::StringTree::StringTreeLexer{};
			const auto stringTree = R"(
			"root" {
				"child0"
				"child1" {
					"subchild0"
				}
			}
			)"_stringTree;
			CppUtils::Graph::logTreeNode(stringTree);

			ASSERT(stringTree.exists("root"s));
			const auto& root = stringTree.at("root"s);
			ASSERT(root.exists("child0"s));
			ASSERT(root.exists("child1"s));
			ASSERT(root.at("child1"s).getChildValue() == "subchild0"s);
		});
	}
}
