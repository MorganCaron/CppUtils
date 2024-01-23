#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTest::Language::HighLevelLabelsCompiler
{
	auto _ = TestSuite{"Language/HighLevelLabelsCompiler", []([[maybe_unused]] auto& suite) {
		using namespace std::literals;
		using Logger = CppUtils::Logger<"CppUtils">;
		namespace VM = CppUtils::Language::VirtualMachine;
		namespace Compiler = CppUtils::Language::HighLevelLabels;

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
			
			auto executionResult = VM::execute<std::size_t>(output);
			suite.expectEqual(executionResult, 42uz);
		});

		suite.addTest("comment", [&] {
			constexpr auto source = uR"(
				(4
				# Comment: 1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ
				2
			)"sv;
			auto [compilationResult, output] = Compiler::compile(source);
			suite.expectEqual(compilationResult, 0uz);
			Logger::print("Output:\n{}\n", CppUtils::String::toAscii(output));
			
			auto executionResult = VM::execute<std::size_t>(output);
			suite.expectEqual(executionResult, 42uz);
		});

		suite.addTest("one label", [&] {
			constexpr auto source = uR"(
				(: (0λ main
				
				21X

				main {
					42X
				}
				
				22X
			)"sv;
			auto [compilationResult, output] = Compiler::compile(source);
			suite.expectEqual(compilationResult, 0uz);
			Logger::print("Output:\n{}\n", CppUtils::String::toAscii(output));
			
			auto executionResult = VM::execute<std::size_t>(output);
			suite.expectEqual(executionResult, 42uz);
		});

		suite.addTest("multiple labels", [&] constexpr {
			constexpr auto source = uR"(
				(21X

				main {
					(: (1λ return40
					# Comment: Add 2
					, 2+X
				}

				(22X

				return40 {
					40
				}

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