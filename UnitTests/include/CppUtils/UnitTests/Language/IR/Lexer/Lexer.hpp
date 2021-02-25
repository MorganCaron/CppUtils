#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::IR::Lexer
{
	const auto tests = std::vector<Test>{

		Test{"Language/IR/Lexer", [] {
			using namespace std::literals;
			const auto irTree = CppUtils::Language::IR::Lexer::parse<int>(R"(
			test()
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
		}}

	};
}
