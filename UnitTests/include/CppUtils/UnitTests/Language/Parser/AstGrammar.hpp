#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Parser::Grammar::AstGrammar
{
	TEST_GROUP("Language/Parser/Grammar/astGrammar")
	{
		using namespace std::literals;
		using namespace CppUtils::Hash::Literals;

		addTest("empty grammar", [] {
			using CppUtils::Language::Parser::AstNode;
			const auto astGrammar = CppUtils::Language::Parser::Ast{"Grammar", {
				AstNode{"main"_token, {
					AstNode{"eof"_token},
					AstNode{"return"_token}
				}}
			}, CppUtils::Language::Parser::getGrammarNames()};
			astGrammar.log();

			auto cursor = CppUtils::Language::Parser::Cursor{""sv};
            const auto result = CppUtils::Language::Parser::parse(astGrammar, cursor);
			result.log();
			
			TEST_ASSERT(std::empty(result.root.nodes));
		});

		addTest("read char", [] {
			using CppUtils::Language::Parser::AstNode;
			const auto astGrammar = CppUtils::Language::Parser::Ast{"Grammar", {
				AstNode{"main"_token, {
					AstNode{"read"_token},
					AstNode{"push"_token},
					AstNode{"increment"_token},
					AstNode{"eof"_token},
					AstNode{"return"_token}
				}}
			}, CppUtils::Language::Parser::getGrammarNames()};
			astGrammar.log();

			auto cursor = CppUtils::Language::Parser::Cursor{"A"sv};
            const auto result = CppUtils::Language::Parser::parse(astGrammar, cursor);
			result.log();
			
			TEST_ASSERT(std::size(result.root.nodes) == 1);
			TEST_ASSERT(result.root.nodes[0].value == 'A');
		});

		addTest("compare chars", [] {
			using CppUtils::Language::Parser::AstNode;
			const auto astGrammar = CppUtils::Language::Parser::Ast{"Grammar", {
				AstNode{"main"_token, {
					AstNode{"set"_token, { AstNode{"read"_token} }},
					AstNode{"call"_token},
					AstNode{"push"_token},
					AstNode{"set"_token, { AstNode{"read"_token} }},
					AstNode{"call"_token},
					AstNode{"compare"_token},
					AstNode{"drop"_token},
					AstNode{"eof"_token},
					AstNode{"return"_token}
				}},
				AstNode{"read"_token, {
					AstNode{"read"_token},
					AstNode{"increment"_token},
					AstNode{"return"_token}
				}}
			}, CppUtils::Language::Parser::getGrammarNames()};
			astGrammar.log();

			auto cursor = CppUtils::Language::Parser::Cursor{"AA"sv};
            const auto result = CppUtils::Language::Parser::parse(astGrammar, cursor);
			result.log();
			
			TEST_ASSERT(std::empty(result.root.nodes));
		});
	}
}
