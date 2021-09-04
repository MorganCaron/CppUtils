#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::ASM::Lexer
{
	TEST_GROUP("Language/ASM/Lexer")
	{
		using namespace std::literals;

		addTest("Nop", [] {
			const auto ast = CppUtils::Language::ASM::Lexer::parse<std::uint64_t>(R"(
			nop
			)"sv);
			CppUtils::Log::TreeNodeLogger::log(ast);
		});
		
		addTest("Halt", [] {
			const auto ast = CppUtils::Language::ASM::Lexer::parse<std::uint64_t>(R"(
			hlt
			)"sv);
			CppUtils::Log::TreeNodeLogger::log(ast);
		});

		addTest("Move", [] {
			const auto ast = CppUtils::Language::ASM::Lexer::parse<std::uint64_t>(R"(
			mov r1, 42
			)"sv);
			CppUtils::Log::TreeNodeLogger::log(ast);
		});
		
		addTest("Add", [] {
			const auto ast = CppUtils::Language::ASM::Lexer::parse<std::uint64_t>(R"(
			mov r2, 20
			add r2, 22
			)"sv);
			CppUtils::Log::TreeNodeLogger::log(ast);
		});

		addTest("Label", [] {
			const auto ast = CppUtils::Language::ASM::Lexer::parse<std::uint64_t>(R"(
			main: mov r2, 20
			add r2, 22
			)"sv);
			CppUtils::Log::TreeNodeLogger::log(ast);
		});
	}
}
