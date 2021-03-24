#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::IR::Compiler
{
	TEST_GROUP("Language/IR/Compiler")
	{
		using namespace std::literals;

		addTest("Operations", [] {
			const auto compiler = CppUtils::Language::IR::Compiler::Compiler<std::int64_t>{};
			const auto context = compiler.compile(R"(
			main()
			{
				nop;
				a = (20 + 24) - 2;
				ret a;
			}
			)"sv);
			CppUtils::Language::IR::Compiler::logContext(context);
		});

		addTest("Pointers", [] {
			const auto compiler = CppUtils::Language::IR::Compiler::Compiler<std::int64_t>{};
			const auto context = compiler.compile(R"(
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
			CppUtils::Language::IR::Compiler::logContext(context);
		});

		addTest("Conditions", [] {
			const auto compiler = CppUtils::Language::IR::Compiler::Compiler<std::int64_t>{};
			const auto context = compiler.compile(R"(
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
			CppUtils::Language::IR::Compiler::logContext(context);
		});
	}
}
