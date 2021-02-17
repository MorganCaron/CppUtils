#pragma once

#include <array>

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::ASM::Lexer
{
	const auto tests = std::vector<Test>{

		Test{"ASM/Lexer/Halt", [] {
			using namespace std::literals;
			const auto ast = CppUtils::ASM::parse<std::int64_t>(R"(
			hlt
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		}},

		Test{"ASM/Lexer/Nop", [] {
			using namespace std::literals;
			const auto ast = CppUtils::ASM::parse<std::int64_t>(R"(
			nop
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		}},

		Test{"ASM/Lexer/Move", [] {
			using namespace std::literals;
			const auto ast = CppUtils::ASM::parse<std::int64_t>(R"(
			mov r0, 20
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		}},
		
		Test{"ASM/Lexer/Add", [] {
			using namespace std::literals;
			const auto ast = CppUtils::ASM::parse<std::int64_t>(R"(
			mov r0, 20
			add r0, 22
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		}}

	};
}
