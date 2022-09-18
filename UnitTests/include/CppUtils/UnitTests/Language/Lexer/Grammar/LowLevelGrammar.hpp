#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Lexer::Grammar::LowLevelGrammar
{
	TEST_GROUP("Language/Lexer/Grammar/lowLevelGrammar")
	{
		using namespace std::literals;
		using namespace CppUtils::Hash::Literals;

		addTest("empty grammar", [] {
			const auto lowLevelGrammar = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammar);
			lowLevelGrammar.log();
			const auto grammar = CppUtils::Language::Lexer::parse(R"(
				main: ;
			)"sv, lowLevelGrammar);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(""sv, grammar);
			TEST_ASSERT(std::size(outputAst.root.nodes) == 0);
		});

		addTest("char", [] {
			const auto lowLevelGrammar = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammar);
			const auto grammar = CppUtils::Language::Lexer::parse(R"(
				main: 'c';
			)"sv, lowLevelGrammar);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammar);
			TEST_ASSERT(std::size(outputAst.root.nodes) == 0);
		});

		addTest("escaped chars", [] {
			const auto lowLevelGrammar = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammar);
			const auto grammar = CppUtils::Language::Lexer::parse(R"(
				main: "\ \n\t\r";
			)"sv, lowLevelGrammar);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(" \n\t\r"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 0);
		});

		addTest("add char", [] {
			const auto lowLevelGrammar = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammar);
			const auto grammar = CppUtils::Language::Lexer::parse(R"(
				main: 'c' add('c');
			)"sv, lowLevelGrammar);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'c');
		});
		
		addTest("read char", [] {
			const auto lowLevelGrammar = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammar);
			const auto grammar = CppUtils::Language::Lexer::parse(R"(
				main: read+;
			)"sv, lowLevelGrammar);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'c');
		});

		addTest("token", [] {
			const auto lowLevelGrammar = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammar);
			const auto grammar = CppUtils::Language::Lexer::parse(R"(
				main: c;
				c: 'c';
			)"sv, lowLevelGrammar);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 0);
		});

		addTest("string", [] {
			const auto lowLevelGrammar = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammar);
			const auto grammar = CppUtils::Language::Lexer::parse(R"(
				main: word;
				word: "Hello World!";
			)"sv, lowLevelGrammar);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("Hello World!"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 0);
		});

		addTest("recurrence", [] {
			const auto lowLevelGrammar = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammar);
			const auto grammar = CppUtils::Language::Lexer::parse(R"(
				main: chars;
				chars: char chars; chars: char;
				char: read+;
			)"sv, lowLevelGrammar);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("AAA"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 3);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'A');
			TEST_ASSERT(outputAst.root.nodes[1].value == 'A');
			TEST_ASSERT(outputAst.root.nodes[2].value == 'A');
		});

		addTest("optional", [] {
			const auto lowLevelGrammar = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammar);
			const auto grammar = CppUtils::Language::Lexer::parse(R"(
				main: chars;
				chars: char ~chars;
				char: read+;
			)"sv, lowLevelGrammar);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("AAA"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 3);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'A');
			TEST_ASSERT(outputAst.root.nodes[1].value == 'A');
			TEST_ASSERT(outputAst.root.nodes[2].value == 'A');
		});

		addTest("or", [] {
			const auto lowLevelGrammar = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammar);
			const auto grammar = CppUtils::Language::Lexer::parse(R"(
				main: or('A' 'B') read+;
			)"sv, lowLevelGrammar);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("BC"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'C');
		});

		addTest("parenthesis", [] {
			const auto lowLevelGrammar = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammar);
			const auto grammar = CppUtils::Language::Lexer::parse(R"(
				main: ('A' 'B') read+;
			)"sv, lowLevelGrammar);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("ABC"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'C');
		});

		addTest("comparison", [] {
			const auto lowLevelGrammar = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammar);
			const auto grammar = CppUtils::Language::Lexer::parse(R"(
				main: char;
				char: or(min maj);
				min: [a, z]+ add(min);
				maj: [A, Z]+ add(maj);
			)"sv, lowLevelGrammar);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("C"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == "maj"_token);
		});

		addTest("hash", [] {
			const auto lowLevelGrammar = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammar);
			const auto grammar = CppUtils::Language::Lexer::parse(R"(
				main: hash(keyword);
				keyword: read+ ~keyword;
			)"sv, lowLevelGrammar);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("variable"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == "variable"_token);
		});

		addTest("not", [] {
			const auto lowLevelGrammar = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammar);
			const auto grammar = CppUtils::Language::Lexer::parse(R"(
				main: quote;
				quote: '"' ~quoteContent '"';
				quoteContent: !'"' read+ ~quoteContent;
			)"sv, lowLevelGrammar);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(R"("Hello")"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 5);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'H');
			TEST_ASSERT(outputAst.root.nodes[1].value == 'e');
			TEST_ASSERT(outputAst.root.nodes[2].value == 'l');
			TEST_ASSERT(outputAst.root.nodes[3].value == 'l');
			TEST_ASSERT(outputAst.root.nodes[4].value == 'o');
		});

		addTest("sub", [] {
			const auto lowLevelGrammar = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammar);
			const auto grammar = CppUtils::Language::Lexer::parse(R"(
				main: node add(;);
				node: read+ ~sub(node);
			)"sv, lowLevelGrammar);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("ABC"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 2);
			auto aNode = outputAst.root.nodes[0];
			TEST_ASSERT(aNode.value == 'A');
			TEST_ASSERT(std::size(aNode.nodes) == 1);
			auto bNode = aNode.nodes[0];
			TEST_ASSERT(bNode.value == 'B');
			TEST_ASSERT(std::size(bNode.nodes) == 1);
			auto cNode = bNode.nodes[0];
			TEST_ASSERT(cNode.value == 'C');
			TEST_ASSERT(outputAst.root.nodes[1].value == ';');
		});
	}
}
