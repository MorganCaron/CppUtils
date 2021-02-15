#pragma once

#include <array>

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::ASM::Lexer
{
	const auto tests = std::vector<Test>{

		Test{"ASM/Lexer/Halt", [] {
			using namespace std::literals;
			CppUtils::Terminal::setConsoleOutputUTF8();
			
			const auto ast = CppUtils::ASM::parse<int>(R"(
			hlt
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		}},

		Test{"ASM/Lexer/Nop", [] {
			using namespace std::literals;
			CppUtils::Terminal::setConsoleOutputUTF8();
			
			const auto ast = CppUtils::ASM::parse<int>(R"(
			nop
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		}},

		Test{"ASM/Lexer/Move", [] {
			using namespace std::literals;
			CppUtils::Terminal::setConsoleOutputUTF8();
			
			const auto ast = CppUtils::ASM::parse<int>(R"(
			mov r0 20
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		}},
		
		Test{"ASM/Lexer/Add", [] {
			using namespace std::literals;
			CppUtils::Terminal::setConsoleOutputUTF8();
			
			const auto ast = CppUtils::ASM::parse<int>(R"(
			mov r0 20
			add r0 22
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		}}

	};
}
