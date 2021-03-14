#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::IR::Lexer
{
	TEST_GROUP("Language/IR/Lexer")
	{
		using namespace std::literals;

		addTest("", [] {
			const auto irTree = CppUtils::Language::IR::Lexer::parse<std::int64_t>(R"(
			main()
			{
				nop;
				a = (20 + 24) - 2;
				ret a;
			}
			)"sv);
			CppUtils::Graph::logTreeNode(irTree);
			ASSERT(irTree.childs.size() == 1);
			ASSERT(irTree.childs.at(0).childs.size() == 1);
			ASSERT(irTree.childs.at(0).childs.at(0).childs.size() == 3);
			ASSERT(irTree.childs.at(0).childs.at(0).childs.at(1).childs.size() == 2);
		});
	}
}
