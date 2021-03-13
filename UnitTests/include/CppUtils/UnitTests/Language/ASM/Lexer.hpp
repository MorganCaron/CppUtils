#pragma once

#include <array>

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::ASM::Lexer
{
	TEST_GROUP("Language/ASM/Lexer")
	{
		using namespace std::literals;

		addTest("Nop", [] {
			const auto ast = CppUtils::Language::ASM::Lexer::parse<Type::Token, std::size_t>(R"(
			nop
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		});
		
		addTest("Halt", [] {
			const auto ast = CppUtils::Language::ASM::Lexer::parse<Type::Token, std::size_t>(R"(
			hlt
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		});

		addTest("Move", [] {
			const auto ast = CppUtils::Language::ASM::Lexer::parse<Type::Token, std::size_t>(R"(
			mov r0, 42
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		});
		
		addTest("Add", [] {
			const auto ast = CppUtils::Language::ASM::Lexer::parse<Type::Token, std::size_t>(R"(
			mov r0, 20
			add r0, 22
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		});

		addTest("Label", [] {
			const auto ast = CppUtils::Language::ASM::Lexer::parse<Type::Token, std::size_t>(R"(
			main: mov r0, 20
			add r0, 22
			)"sv);
			CppUtils::Graph::logTreeNode(ast);
		});
	}
}
