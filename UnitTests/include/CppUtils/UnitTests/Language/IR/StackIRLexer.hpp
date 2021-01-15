#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::IR::StackIRLexer
{
	const auto tests = std::vector<Test>{

		Test{"Language/IR/StackIRLexer", [] {
			using namespace std::literals;
			const auto stackIRTree = CppUtils::Language::IR::StackIR::parse<float>(R"(
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
		}}

	};
}
