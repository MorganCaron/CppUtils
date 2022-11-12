#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Lexer::Grammar::HighLevelGrammar
{
	TEST_GROUP("Language/Lexer/Grammar/highLevelGrammar")
	{
		using namespace std::literals;
		using namespace CppUtils::Hash::Literals;
		using namespace CppUtils::Language::Lexer::Grammar::Literals;

		addTest("empty grammar", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			highLevelGrammarAst.log();
			
			const auto grammarAst = R"(
				main: ;
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(""sv, grammarAst);
			
			TEST_ASSERT(std::empty(outputAst.root.nodes));
		});

		addTest("end", [] {
			const auto grammarAst = R"(
				main: end();
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(""sv, grammarAst);
			
			TEST_ASSERT(std::empty(outputAst.root.nodes));
		});

		addTest("comment", [] {
			const auto grammarAst = R"(
				# entry point
				main: ;
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(""sv, grammarAst);
			
			TEST_ASSERT(std::empty(outputAst.root.nodes));
		});

		addTest("char", [] {
			const auto grammarAst = R"(
				main: 'c';
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammarAst);
			
			TEST_ASSERT(std::empty(outputAst.root.nodes));
		});

		addTest("is char", [] {
			const auto grammarAst = R"(
				main: is('c') 'c';
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammarAst);
			
			TEST_ASSERT(std::empty(outputAst.root.nodes));
		});

		addTest("escaped chars", [] {
			const auto grammarAst = R"(
				main: "\ \n\t\r\"";
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(" \n\t\r\""sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(std::empty(outputAst.root.nodes));
		});

		addTest("add char", [] {
			const auto grammarAst = R"(
				main: 'c' add('c');
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(CppUtils::Language::Parser::getString(outputAst.root) == "c");
		});

		addTest("read char", [] {
			const auto grammarAst = R"(
				main: read()+;
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(CppUtils::Language::Parser::getString(outputAst.root) == "c");
		});

		addTest("token", [] {
			const auto grammarAst = R"(
				main: c;
				c: 'c';
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(std::empty(outputAst.root.nodes));
		});

		addTest("string", [] {
			const auto grammarAst = R"(
				main: word;
				word: "Hello World!";
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("Hello World!"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(std::empty(outputAst.root.nodes));
		});

		addTest("recurrence", [] {
			const auto grammarAst = R"(
				main: chars;
				chars: char chars; chars: char;
				char: read()+;
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("AAA"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(CppUtils::Language::Parser::getString(outputAst.root) == "AAA");
		});

		addTest("optional", [] {
			const auto grammarAst = R"(
				main: chars;
				chars: char ~chars;
				char: read()+;
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("AAA"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(CppUtils::Language::Parser::getString(outputAst.root) == "AAA");
		});

		addTest("or", [] {
			const auto grammarAst = R"(
				main: or('A', 'B') read()+;
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("BC"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(CppUtils::Language::Parser::getString(outputAst.root) == "C");
		});

		addTest("parenthesis", [] {
			const auto grammarAst = R"(
				main: ('A' 'B') read()+;
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("ABC"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(CppUtils::Language::Parser::getString(outputAst.root) == "C");
		});

		addTest("comparison", [] {
			const auto grammarAst = R"(
				main: char;
				char: or(min, maj);
				min: [a, z]+ add(min);
				maj: [A, Z]+ add(maj);
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("C"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == "maj"_token);
		});

		addTest("hash", [] {
			const auto grammarAst = R"(
				main: hash(keyword);
				keyword: read()+ ~keyword;
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("variable"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == "variable"_token);
		});

		addTest("not", [] {
			const auto grammarAst = R"(
				main: quote;
				quote: '"' ~quoteContent '"';
				quoteContent: !'"' read()+ ~quoteContent;
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(R"("Hello")"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(CppUtils::Language::Parser::getString(outputAst.root) == "Hello");
		});

		addTest("sub", [] {
			const auto grammarAst = R"(
				main: node add(';');
				node: read()+ ~sub(node);
			)"_grammar;
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
			const auto grammarAst = R"(
				main: repeat('A') repeat('B', ',') read()+;
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("AAAB,B,BC"sv, grammarAst);
			outputAst.log();
			
			TEST_ASSERT(CppUtils::Language::Parser::getString(outputAst.root) == "C");
		});

		addTest("push-pop", [] {
			const auto grammarAst = R"(
				main: push(hash(keyword)) pop();
				keyword: read()+ ~keyword;
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("ABC"sv, grammarAst);
			outputAst.log();

			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == "ABC"_token);
		});
	}
}
