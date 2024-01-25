#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::Language::LowLevelLabelsCompiler
{
	auto _ = TestSuite{"Language/LowLevelLabelsCompiler", []([[maybe_unused]] auto& suite) {
		using namespace std::literals;
		using Logger = CppUtils::Logger<"CppUtils">;
		namespace VM = CppUtils::Language::VirtualMachine;
		namespace Compiler = CppUtils::Language::LowLevelLabels;

		suite.addTest("empty source", [&] {
			constexpr auto source = u"("sv;
			auto [compilationResult, output] = Compiler::compile(source);
			suite.expectEqual(compilationResult, 0uz);
			Logger::print("Output:\n{}\n", CppUtils::String::toAscii(output));

			auto executionResult = VM::execute<std::size_t>(output);
			suite.expectEqual(executionResult, 0uz);
		});

		suite.addTest("code without labels", [&] {
			constexpr auto source = uR"(
				(42X
			)"sv;
			auto [compilationResult, output] = Compiler::compile(source);
			suite.expectEqual(compilationResult, 0uz);
			Logger::print("Output:\n{}\n", CppUtils::String::toAscii(output));
			suite.expectEqual(output, source);

			auto executionResult = VM::execute<std::size_t>(output);
			suite.expectEqual(executionResult, 42uz);
		});

		suite.addTest("one label", [&] {
			constexpr auto source = uR"(
				(0¤ main
				
				(21X

				§ main
					(42X

				(22X
			)"sv;
			auto [compilationResult, output] = Compiler::compile(source);
			suite.expectEqual(compilationResult, 0uz);
			Logger::print("Output:\n{}\n", CppUtils::String::toAscii(output));

			auto executionResult = VM::execute<std::size_t>(output);
			suite.expectEqual(executionResult, 42uz);
		});

		suite.addTest("multiple labels", [&] constexpr {
			constexpr auto source = uR"(
				(0¤ main
				
				(21X

				§ main
					(1¤ function

				(22X

				§ function
					(42X

				(23X
			)"sv;
			auto [compilationResult, output] = Compiler::compile(source);
			suite.expectEqual(compilationResult, 0uz);
			Logger::print("Output:\n{}\n", CppUtils::String::toAscii(output));

			auto executionResult = VM::execute<std::size_t>(output);
			suite.expectEqual(executionResult, 42uz);
		});
	}};
}
