#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::IR::Compiler
{
	TEST_GROUP("Language/IR/Compiler")
	{
		using namespace std::literals;

		addTest("Operations", [] {
			const auto compiler = CppUtils::Language::IR::Compiler::Compiler<std::uint64_t>{};
			const auto output = compiler.compile(R"(
			int main()
			{
				nop;
				a = (20 + 24) - 2;
				return a;
			}
			)"sv);
			output.log();
		});

		addTest("Pointers", [] {
			const auto compiler = CppUtils::Language::IR::Compiler::Compiler<std::uint64_t>{};
			const auto output = compiler.compile(R"(
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
			output.log();
		});

		addTest("Conditions", [] {
			const auto compiler = CppUtils::Language::IR::Compiler::Compiler<std::uint64_t>{};
			const auto output = compiler.compile(R"(
			int getLength(text)
			{
				length = 0;
				while ((text == 0) == 0)
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
			output.log();
		});
	}
}
