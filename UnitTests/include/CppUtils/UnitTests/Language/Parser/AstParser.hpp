#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Parser
{
	TEST_GROUP("Language/AstParser")
	{
		using namespace std::literals;
		using namespace CppUtils::Hash::Literals;

		addTest("parseAst", [] {
			const auto ast = CppUtils::Language::Parser::parseAst(R"(
				base_{
					group0_{ element0_ element1_ }
					group1_{ subGroup_{ Hello\ World!\n } }
				}
			)"sv);

			TEST_ASSERT(ast.root.value == "root"_token);
			TEST_ASSERT(std::size(ast.root.nodes) == 1);

			const auto& baseNode = ast.root.nodes[0];
			TEST_ASSERT(baseNode.value == "base"_token);
			TEST_ASSERT(std::size(baseNode.nodes) == 2);

			const auto& group0Node = baseNode.nodes[0];
			TEST_ASSERT(group0Node.value == "group0"_token);
			TEST_ASSERT(std::size(group0Node.nodes) == 2);
			TEST_ASSERT(group0Node.nodes[0].value == "element0"_token);
			TEST_ASSERT(group0Node.nodes[1].value == "element1"_token);

			const auto& group1Node = baseNode.nodes[1];
			TEST_ASSERT(group1Node.value == "group1"_token);
			TEST_ASSERT(std::size(group1Node.nodes) == 1);
			TEST_ASSERT(group1Node.nodes[0].value == "subGroup"_token);

			const auto& subGroupNode = group1Node.nodes[0];
			TEST_ASSERT(std::size(subGroupNode.nodes) == 13);
			TEST_ASSERT(subGroupNode.nodes[0].value == 'H');
			TEST_ASSERT(subGroupNode.nodes[1].value == 'e');
			TEST_ASSERT(subGroupNode.nodes[2].value == 'l');
			TEST_ASSERT(subGroupNode.nodes[3].value == 'l');
			TEST_ASSERT(subGroupNode.nodes[4].value == 'o');
			TEST_ASSERT(subGroupNode.nodes[5].value == ' ');
			TEST_ASSERT(subGroupNode.nodes[6].value == 'W');
			TEST_ASSERT(subGroupNode.nodes[7].value == 'o');
			TEST_ASSERT(subGroupNode.nodes[8].value == 'r');
			TEST_ASSERT(subGroupNode.nodes[9].value == 'l');
			TEST_ASSERT(subGroupNode.nodes[10].value == 'd');
			TEST_ASSERT(subGroupNode.nodes[11].value == '!');
			TEST_ASSERT(subGroupNode.nodes[12].value == '\n');
		});
	}
}
