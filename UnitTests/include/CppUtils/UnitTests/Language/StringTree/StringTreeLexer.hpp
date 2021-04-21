#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::StringTree::StringTreeLexer
{
	TEST_GROUP("Language/StringTree/StringTreeLexer")
	{
		using namespace CppUtils::Type::Literals;
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

			ASSERT(stringTree.exists("root"_token));
			const auto& root = stringTree.at("root"_token);
			ASSERT(root.exists("child0"_token));
			ASSERT(root.exists("child1"_token));
			ASSERT(root.at("child1"_token).getChildValue() == "subchild0"_token);
		});
	}
}
