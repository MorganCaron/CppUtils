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
			
			ASSERT(std::get<CppUtils::Type::Token>(stringTree.value) == "main"_token);
			ASSERT(stringTree.childs.size() == 1);
			ASSERT(std::get<CppUtils::Type::Token>(stringTree.childs.at(0).value) == "root"_token);
			ASSERT(stringTree.childs.at(0).childs.size() == 2);
			ASSERT(std::get<CppUtils::Type::Token>(stringTree.childs.at(0).childs.at(0).value) == "child0"_token);
			ASSERT(stringTree.childs.at(0).childs.at(0).childs.size() == 0);
			ASSERT(std::get<CppUtils::Type::Token>(stringTree.childs.at(0).childs.at(1).value) == "child1"_token);
			ASSERT(stringTree.childs.at(0).childs.at(1).childs.size() == 1);
			ASSERT(std::get<CppUtils::Type::Token>(stringTree.childs.at(0).childs.at(1).childs.at(0).value) == "subchild0"_token);
			ASSERT(stringTree.childs.at(0).childs.at(1).childs.at(0).childs.size() == 0);
		});
	}
}
