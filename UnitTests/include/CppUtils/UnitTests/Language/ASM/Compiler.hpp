#pragma once

#include <array>

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::ASM::Compiler
{
	TEST_GROUP("Language/ASM/Compiler")
	{
		using namespace std::literals;

		addTest("Nop", [] {
			const auto compiler = CppUtils::Language::ASM::Compiler::Compiler<std::int64_t>{};
			const auto context = compiler.compile(R"(
			nop
			)"sv);
			for (const auto& instruction : context.instructions)
				std::cout << *instruction << std::endl;
		});

		addTest("Halt", [] {
			const auto compiler = CppUtils::Language::ASM::Compiler::Compiler<std::int64_t>{};
			const auto context = compiler.compile(R"(
			hlt
			)"sv);
			for (const auto& instruction : context.instructions)
				std::cout << *instruction << std::endl;
		});

		addTest("Move", [] {
			const auto compiler = CppUtils::Language::ASM::Compiler::Compiler<std::int64_t>{};
			const auto context = compiler.compile(R"(
			mov r1, 42
			)"sv);
			for (const auto& instruction : context.instructions)
				std::cout << *instruction << std::endl;
		});

		addTest("Add", [] {
			const auto compiler = CppUtils::Language::ASM::Compiler::Compiler<std::int64_t>{};
			const auto context = compiler.compile(R"(
			mov r2, 20
			add r2, 22
			)"sv);
			for (const auto& instruction : context.instructions)
				std::cout << *instruction << std::endl;
		});

		addTest("Label", [] {
			const auto compiler = CppUtils::Language::ASM::Compiler::Compiler<std::int64_t>{};
			const auto context = compiler.compile(R"(
			main: mov r2, 20
			add r2, 22
			)"sv);
			for (const auto& instruction : context.instructions)
				std::cout << *instruction << std::endl;
		});
	}
}
