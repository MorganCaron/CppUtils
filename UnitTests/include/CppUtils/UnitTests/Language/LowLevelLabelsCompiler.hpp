#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::Language::LowLevelLabelsCompiler
{
	auto _ = TestSuite{"Language/LowLevelLabelsCompiler", []([[maybe_unused]] auto& suite) {
		using namespace std::literals;
		namespace VM = CppUtils::Language::VirtualMachine;

		suite.addTest("empty source", [&] {
			constexpr auto source = u8""sv;
			auto [result, output] = CppUtils::Language::LowLevelLabels::compile(source);
			suite.expectEqual(result, 0uz);
			suite.expectEqual(std::size(output), 0uz);
		});

		suite.addTest("code without labels", [&] {
			constexpr auto source = u8R"(
				42X
			)"sv;
			auto [result, output] = CppUtils::Language::LowLevelLabels::compile(source);
			suite.expectEqual(result, 0uz);
			suite.expectEqual(output, source);
		});

		suite.addTest("one label", [&] {
			constexpr auto source = u8R"(
				_21X
				
				_0¤ goto main
				
				_22X

				§ main
					_42X
				
				_23X
			)"sv;
			auto [compilationResult, output] = CppUtils::Language::LowLevelLabels::compile(source);
			suite.expectEqual(compilationResult, 0uz);
			auto executionResult = VM::execute<std::size_t>(output);
			suite.expectEqual(executionResult, 42uz);
		});

		suite.addTest("multiple labels", [&] constexpr {
			// todo
		});
		
	}};
}
