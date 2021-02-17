#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::StringTree::StringTreeLexer
{
	const auto tests = std::vector<CppUtils::Test>{

		CppUtils::Test{"StringTree/StringTreeLexer/parse", [] {
			using namespace CppUtils::StringTree::Literals;
			using namespace CppUtils::Type::Literals;
			const auto stringTreeLexer = CppUtils::StringTree::StringTreeLexer{};
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
		}}

	};
}
