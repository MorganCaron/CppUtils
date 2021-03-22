#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::IR::Lexer
{
	TEST_GROUP("Language/IR/Lexer")
	{
		using namespace std::literals;

		addTest("Operations", [] {
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
			ASSERT(irTree.childs.at(0).childs.size() == 2);
			ASSERT(irTree.childs.at(0).childs.at(1).childs.size() == 3);
		});

		addTest("Pointers", [] {
			const auto irTree = CppUtils::Language::IR::Lexer::parse<std::int64_t>(R"(
			append(output, c)
			{
				*output = c;
				output += 1;
				*output = 0;
				ret output;
			}
			
			main()
			{
				ret append("hello", '!');
			}
			)"sv);
			CppUtils::Graph::logTreeNode(irTree);
			ASSERT(irTree.childs.size() == 2);
			ASSERT(irTree.childs.at(0).childs.size() == 2);
			ASSERT(irTree.childs.at(0).childs.at(1).childs.size() == 4);
		});

		addTest("Conditions", [] {
			const auto irTree = CppUtils::Language::IR::Lexer::parse<std::int64_t>(R"(
			getLength(text)
			{
				length = 0;
				while ((text == 0) == 0)
				{
					length += 1;
					text += 1;
				}
				ret length;
			}

			main()
			{
				text = "Hello World!";
				ret getLength(text);
			}
			)"sv);
			CppUtils::Graph::logTreeNode(irTree);
			ASSERT(irTree.childs.size() == 2);
			ASSERT(irTree.childs.at(0).childs.size() == 2);
			ASSERT(irTree.childs.at(0).childs.at(1).childs.size() == 3);
		});
	}
}
