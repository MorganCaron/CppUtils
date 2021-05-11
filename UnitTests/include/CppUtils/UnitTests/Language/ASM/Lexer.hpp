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
			CppUtils::Graph::logTreeNode(ast);
		});
		
		addTest("Halt", [] {
			const auto ast = CppUtils::Language::ASM::Lexer::parse<std::uint64_t>(R"(
			hlt
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		});

		addTest("Move", [] {
			const auto ast = CppUtils::Language::ASM::Lexer::parse<std::uint64_t>(R"(
			mov r1, 42
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		});
		
		addTest("Add", [] {
			const auto ast = CppUtils::Language::ASM::Lexer::parse<std::uint64_t>(R"(
			mov r2, 20
			add r2, 22
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		});

		addTest("Label", [] {
			const auto ast = CppUtils::Language::ASM::Lexer::parse<std::uint64_t>(R"(
			main: mov r2, 20
			add r2, 22
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		});
	}
}
