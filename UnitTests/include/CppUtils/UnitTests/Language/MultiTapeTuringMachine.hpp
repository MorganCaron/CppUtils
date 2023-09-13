#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::MultiTapeTuringMachine
{
	TEST_GROUP("Language/MultiTapeTuringMachine")
	{
		using namespace std::literals;

		addTest("multiTapeTuringMachine", [] {
			constexpr const auto source = u8R"(
				_3<_L
			)"sv;

			constexpr const auto input = u8"Hello World!"sv;

			EXPECT(CppUtils::Language::multiTapeTuringMachine(source, input));
		});
	}
}
