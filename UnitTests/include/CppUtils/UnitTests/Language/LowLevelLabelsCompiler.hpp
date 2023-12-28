#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::Language::LowLevelLabelsCompiler
{
	auto _ = TestSuite{"Language/LowLevelLabelsCompiler", []([[maybe_unused]] auto& suite) {
		using namespace std::literals;

		suite.addTest("empty source", [&] constexpr {
			constexpr auto source = u8""sv;
			auto [result, output] = CppUtils::Language::LowLevelLabels::compile(source);
			suite.expectEqual(result, 0uz);
			suite.expectEqual(std::size(output), 0uz);
		});

		/*suite.addTest("code without labels", [&] constexpr {
			constexpr auto source = "42"sv;
			constexpr auto output = ""sv;
			auto result = CppUtils::Language::LowLevelLabels::compile(source, output);
			suite.expectEqual(result, 0uz);
			suite.expectEqual(output, "42"sv);
		});*/
		
	}};
}
