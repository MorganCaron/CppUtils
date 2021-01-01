#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Lexer::StringTreeLexer
{
	const auto tests = std::vector<CppUtils::Test>{

		CppUtils::Test{"Language/Lexer/StringTreeLexer/parse", [] {
			using namespace CppUtils::Type::Literals;

			const auto stringTreeLexer = CppUtils::Language::Lexer::StringTreeLexer{};
			const auto stringTree = stringTreeLexer.parse(R"(
			"root" {
				"child0"
				"child1" {
					"subchild0"
				}
			}
			)");

			CppUtils::Graph::logTreeNode(stringTree);
			
			ASSERT(stringTree.self == "main"_token);
			ASSERT(stringTree.childs.size() == 1);
			ASSERT(stringTree.childs.at(0).self == "root"_token);
			ASSERT(stringTree.childs.at(0).childs.size() == 2);
			ASSERT(stringTree.childs.at(0).childs.at(0).self == "child0"_token);
			ASSERT(stringTree.childs.at(0).childs.at(0).childs.size() == 0);
			ASSERT(stringTree.childs.at(0).childs.at(1).self == "child1"_token);
			ASSERT(stringTree.childs.at(0).childs.at(1).childs.size() == 1);
			ASSERT(stringTree.childs.at(0).childs.at(1).childs.at(0).self == "subchild0"_token);
			ASSERT(stringTree.childs.at(0).childs.at(1).childs.at(0).childs.size() == 0);
		}}

	};
}
