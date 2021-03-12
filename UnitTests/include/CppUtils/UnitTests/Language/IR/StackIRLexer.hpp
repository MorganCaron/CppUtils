#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::IR::StackIRLexer
{
	TEST_GROUP("Language/IR/StackIRLexer")
	{
		addTest("", [] {
			using namespace std::literals;
			const auto stackIRTree = CppUtils::Language::IR::StackIR::parse<Type::Token, float>(R"(
			load b
			load c
			add
			load b
			load c
			add
			mul
			store a
			)"sv);
			CppUtils::Graph::logTreeNode(stackIRTree);
			ASSERT(stackIRTree.childs.size() == 8);
		});
	}
}
