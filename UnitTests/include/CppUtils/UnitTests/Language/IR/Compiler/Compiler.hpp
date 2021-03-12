#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::IR::Compiler
{
	TEST_GROUP("Language/IR/Compiler")
	{
		addTest("", [] {
			using namespace std::literals;
			const auto irTree = CppUtils::Language::IR::Lexer::parse<Type::Token, int>(R"(
			main()
			{
				nop;
				a = (10 + 12) * 2;
				ret a;
			}
			)"sv);
			CppUtils::Graph::logTreeNode(irTree);
			ASSERT(irTree.childs.size() == 1);
			ASSERT(irTree.childs.at(0).childs.size() == 3);
			ASSERT(irTree.childs.at(0).childs.at(1).childs.size() == 2);
		});
	}
}
