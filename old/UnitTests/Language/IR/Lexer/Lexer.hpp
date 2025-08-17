#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::IR::Lexer
{
	TEST_GROUP("Language/IR/Lexer")
	{
		using namespace std::literals;

		addTest("Operations", [] {
			const auto irTree = CppUtils::Language::IR::Lexer::parse(R"(
			int main()
			{
				nop;
				a = (20 + 24) - 2;
				return a;
			}
			)"sv);

			CppUtils::Log::TreeNodeLogger::log(irTree);
			EXPECT(irTree.children.size() == 1);
			EXPECT(irTree.children[0].children.size() == 3);
			EXPECT(irTree.children[0].children[2].children.size() == 3);
		});

		addTest("Pointers", [] {
			const auto irTree = CppUtils::Language::IR::Lexer::parse(R"(
			address append(output, c)
			{
				*output = c;
				output += 1;
				*output = 0;
				return output;
			}
			
			int main()
			{
				return append("hello", '!');
			}
			)"sv);
			CppUtils::Log::TreeNodeLogger::log(irTree);
			EXPECT(irTree.children.size() == 2);
			EXPECT(irTree.children[0].children.size() == 3);
			EXPECT(irTree.children[0].children.at(2).children.size() == 4);
		});

		addTest("Conditions", [] {
			const auto irTree = CppUtils::Language::IR::Lexer::parse(R"(
			int getLength(text)
			{
				length = 0;
				while ((*text == 0) == 0)
				{
					length += 1;
					text += 1;
				}
				return length;
			}

			int main()
			{
				text = "Hello World!";
				return getLength(text);
			}
			)"sv);
			CppUtils::Log::TreeNodeLogger::log(irTree);
			EXPECT(irTree.children.size() == 2);
			EXPECT(irTree.children[0].children.size() == 3);
			EXPECT(irTree.children[0].children[2].children.size() == 3);
		});
	}
}
