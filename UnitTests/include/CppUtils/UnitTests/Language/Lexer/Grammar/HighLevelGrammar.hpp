#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Lexer::Grammar::HighLevelGrammar
{
	TEST_GROUP("Language/Lexer/Grammar/highLevelGrammar")
	{
		using namespace std::literals;
		using namespace CppUtils::Hash::Literals;

		addTest("empty grammar", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			highLevelGrammarAst.log();
			
			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
				main: ;
			)"sv, highLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(""sv, grammarAst);
			
			TEST_ASSERT(std::empty(outputAst.root.nodes));
		});

		addTest("end", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			
			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
				main: end;
			)"sv, highLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(""sv, grammarAst);
			
			TEST_ASSERT(std::empty(outputAst.root.nodes));
		});

		addTest("comment", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			
			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
				# entry point
				main: ;
			)"sv, highLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(""sv, grammarAst);
			
			TEST_ASSERT(std::empty(outputAst.root.nodes));
		});

		addTest("char", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			
			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
				main: 'c';
			)"sv, highLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammarAst);
			
			TEST_ASSERT(std::empty(outputAst.root.nodes));
		});

		addTest("escaped chars", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			
			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
				main: "\ \n\t\r\"";
			)"sv, highLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(" \n\t\r\""sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(std::empty(outputAst.root.nodes));
		});

		addTest("add char", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			
			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
				main: 'c' add('c');
			)"sv, highLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'c');
		});

		addTest("read char", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			
			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
				main: read+;
			)"sv, highLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'c');
		});

		addTest("token", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			
			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
				main: c;
				c: 'c';
			)"sv, highLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(std::empty(outputAst.root.nodes));
		});

		addTest("string", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			
			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
				main: word;
				word: "Hello World!";
			)"sv, highLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("Hello World!"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(std::empty(outputAst.root.nodes));
		});

		addTest("recurrence", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			
			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
				main: chars;
				chars: char chars; chars: char;
				char: read+;
			)"sv, highLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("AAA"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(std::size(outputAst.root.nodes) == 3);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'A');
			TEST_ASSERT(outputAst.root.nodes[1].value == 'A');
			TEST_ASSERT(outputAst.root.nodes[2].value == 'A');
		});

		addTest("optional", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			
			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
				main: chars;
				chars: char ~chars;
				char: read+;
			)"sv, highLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("AAA"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(std::size(outputAst.root.nodes) == 3);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'A');
			TEST_ASSERT(outputAst.root.nodes[1].value == 'A');
			TEST_ASSERT(outputAst.root.nodes[2].value == 'A');
		});

		addTest("or", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			
			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
				main: or('A', 'B') read+;
			)"sv, highLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("BC"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'C');
		});

		addTest("parenthesis", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			
			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
				main: ('A' 'B') read+;
			)"sv, highLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("ABC"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'C');
		});

		addTest("comparison", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			
			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
				main: char;
				char: or(min, maj);
				min: [a, z]+ add(min);
				maj: [A, Z]+ add(maj);
			)"sv, highLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("C"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == "maj"_token);
		});

		addTest("hash", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			
			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
				main: hash(keyword);
				keyword: read+ ~keyword;
			)"sv, highLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("variable"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == "variable"_token);
		});

		addTest("not", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			
			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
				main: quote;
				quote: '"' ~quoteContent '"';
				quoteContent: !'"' read+ ~quoteContent;
			)"sv, highLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(R"("Hello")"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(std::size(outputAst.root.nodes) == 5);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'H');
			TEST_ASSERT(outputAst.root.nodes[1].value == 'e');
			TEST_ASSERT(outputAst.root.nodes[2].value == 'l');
			TEST_ASSERT(outputAst.root.nodes[3].value == 'l');
			TEST_ASSERT(outputAst.root.nodes[4].value == 'o');
		});

		addTest("sub", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			
			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
				main: node add(';');
				node: read+ ~sub(node);
			)"sv, highLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("ABC"sv, grammarAst);
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
			TEST_ASSERT(std::empty(cNode.nodes));
			TEST_ASSERT(outputAst.root.nodes[1].value == ';');
		});

		addTest("repeat", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			
			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
				main: repeat('A') repeat('B', ',') read+;
			)"sv, highLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("AAAB,B,BC"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'C');
		});
	}
}
