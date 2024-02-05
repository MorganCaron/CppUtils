#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Grammar::AstGrammar
{
	TEST_GROUP("Language/Parser/Grammar/AstGrammar")
	{
		using namespace std::literals;
		using namespace CppUtils::String::Literals;

		using CppUtils::Language::Ast;
		using CppUtils::Language::AstNode;

		addTest("empty grammar", [] {
			constexpr auto source = ""sv;

			auto context = CppUtils::Language::Grammar::getAstGrammar();
			CppUtils::Language::Ast::pushPointer(context.root["rodata"_token], std::data(source));

			CppUtils::Language::VirtualMachine::run(context);
			context.log();

			const auto& result = CppUtils::Language::VirtualMachine::getReturnNode(context);
			EXPECT(result == AstNode{0});
		});

		addTest("single node", [] {
			constexpr auto source = "node"sv;

			auto context = CppUtils::Language::Grammar::getAstGrammar();
			CppUtils::Language::Ast::pushPointer(context.root["rodata"_token], std::data(source));

			CppUtils::Language::VirtualMachine::run(context);
			context.log();

			const auto& result = CppUtils::Language::VirtualMachine::getReturnNode(context);
			EXPECT(result == AstNode{0});
		});

		addTest("whitespaces", [] {
			constexpr auto source = R"(
				node
			)"sv;

			auto context = CppUtils::Language::Grammar::getAstGrammar();
			CppUtils::Language::Ast::pushPointer(context.root["rodata"_token], std::data(source));

			CppUtils::Language::VirtualMachine::run(context);
			context.log();

			const auto& result = CppUtils::Language::VirtualMachine::getReturnNode(context);
			EXPECT(result == AstNode{0});
		});

		addTest("multiple nodes", [] {
			constexpr auto source = "Hello World"sv;

			auto context = CppUtils::Language::Grammar::getAstGrammar();
			CppUtils::Language::Ast::pushPointer(context.root["rodata"_token], std::data(source));

			CppUtils::Language::VirtualMachine::run(context);
			context.log();

			const auto& result = CppUtils::Language::VirtualMachine::getReturnNode(context);
			EXPECT(result == AstNode{0});
		});

		addTest("escaped chars", [] {
			constexpr auto source = "\n \t \r Hello\\ World!"sv;

			auto context = CppUtils::Language::Grammar::getAstGrammar();
			CppUtils::Language::Ast::pushPointer(context.root["rodata"_token], std::data(source));

			CppUtils::Language::VirtualMachine::run(context);
			context.log();

			const auto& result = CppUtils::Language::VirtualMachine::getReturnNode(context);
			EXPECT(result == AstNode{0});
		});

		addTest("node with empty content", [] {
			constexpr auto source = "node{}"sv;

			auto context = CppUtils::Language::Grammar::getAstGrammar();
			CppUtils::Language::Ast::pushPointer(context.root["rodata"_token], std::data(source));

			CppUtils::Language::VirtualMachine::run(context);
			context.log();

			const auto& result = CppUtils::Language::VirtualMachine::getReturnNode(context);
			EXPECT(result == AstNode{0});
		});

		addTest("node with content", [] {
			constexpr auto source = "string{Hello World}"sv;

			auto context = CppUtils::Language::Grammar::getAstGrammar();
			CppUtils::Language::Ast::pushPointer(context.root["rodata"_token], std::data(source));

			CppUtils::Language::VirtualMachine::run(context);
			context.log();

			const auto& result = CppUtils::Language::VirtualMachine::getReturnNode(context);
			EXPECT(result == AstNode{0});
		});

		addTest("nested nodes", [] {
			constexpr auto source = R"(
				main{
					print{ string{Hello World \0} }
				}
			)"sv;

			auto context = CppUtils::Language::Grammar::getAstGrammar();
			CppUtils::Language::Ast::pushPointer(context.root["rodata"_token], std::data(source));

			CppUtils::Language::VirtualMachine::run(context);
			context.log();

			const auto& result = CppUtils::Language::VirtualMachine::getReturnNode(context);
			EXPECT(result == AstNode{0});
		});
	}
}
