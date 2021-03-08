#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::IR::RegisterIRLexer
{
	const auto tests = std::vector<Test>{

		Test{"Language/IR/RegisterIRLexer", [] {
			using namespace std::literals;
			const auto registerIRTree = CppUtils::Language::IR::RegisterIR::parse<Type::Token, float>(R"(
			r0 = load b
			r1 = load c
			r2 = r0 + r1
			r3 = load b
			r4 = load c
			r5 = r3 + r4
			r6 = r2 * r5
			store a r6
			)"sv);
			CppUtils::Graph::logTreeNode(registerIRTree);

			ASSERT(registerIRTree.childs.size() == 8);
		}}

	};
}
