#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::IR::Compiler
{
	TEST_GROUP("Language/IR/Compiler")
	{
		using namespace std::literals;

		addTest("", [] {
			const auto compiler = CppUtils::Language::IR::Compiler::Compiler<std::int64_t>{};
			const auto context = compiler.compile(R"(
			main()
			{
				nop;
				a = (20 + 24) - 2;
				ret a;
			}
			)"sv);
			for (const auto& instruction : context.instructions)
				std::cout << *instruction << std::endl;
		});
	}
}
