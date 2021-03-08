#pragma once

#include <array>

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::ASM::Lexer
{
	const auto tests = std::vector<Test>{

		Test{"Language/ASM/Lexer/Nop", [] {
			using namespace std::literals;
			const auto ast = CppUtils::Language::ASM::Lexer::parse<Type::Token, std::size_t>(R"(
			nop
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		}},
		
		Test{"Language/ASM/Lexer/Halt", [] {
			using namespace std::literals;
			const auto ast = CppUtils::Language::ASM::Lexer::parse<Type::Token, std::size_t>(R"(
			hlt
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		}},

		Test{"Language/ASM/Lexer/Move", [] {
			using namespace std::literals;
			const auto ast = CppUtils::Language::ASM::Lexer::parse<Type::Token, std::size_t>(R"(
			mov r0, 20
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		}},
		
		Test{"Language/ASM/Lexer/Add", [] {
			using namespace std::literals;
			const auto ast = CppUtils::Language::ASM::Lexer::parse<Type::Token, std::size_t>(R"(
			mov r0, 20
			add r0, 22
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		}}

	};
}
