#pragma once

#include <array>

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::ASM::Compiler
{
	const auto tests = std::vector<Test>{

		Test{"Language/ASM/Compiler/Nop", [] {
			using namespace std::literals;
			const auto instructions = CppUtils::Language::ASM::Compiler::compile<std::int64_t>(R"(
			nop
			)"sv);
			for (const auto& instruction : instructions)
				std::cout << *instruction << std::endl;
		}},
		
		Test{"Language/ASM/Compiler/Halt", [] {
			using namespace std::literals;
			const auto instructions = CppUtils::Language::ASM::Compiler::compile<std::int64_t>(R"(
			hlt
			)"sv);
			for (const auto& instruction : instructions)
				std::cout << *instruction << std::endl;
		}},

		Test{"Language/ASM/Compiler/Move", [] {
			using namespace std::literals;
			const auto instructions = CppUtils::Language::ASM::Compiler::compile<std::int64_t>(R"(
			mov r0, 20
			)"sv);
			for (const auto& instruction : instructions)
				std::cout << *instruction << std::endl;
		}},
		
		Test{"Language/ASM/Compiler/Add", [] {
			using namespace std::literals;
			const auto instructions = CppUtils::Language::ASM::Compiler::compile<std::int64_t>(R"(
			mov r0, 20
			add r0, 22
			)"sv);
			for (const auto& instruction : instructions)
				std::cout << *instruction << std::endl;
		}}

	};
}
