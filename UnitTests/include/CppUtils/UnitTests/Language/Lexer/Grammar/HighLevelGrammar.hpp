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
			
			EXPECT(std::empty(outputAst.root.nodes));
		});

		addTest("end", [] {
			const auto grammarAst = R"(
				main: end();
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(""sv, grammarAst);
			
			EXPECT(std::empty(outputAst.root.nodes));
		});

		addTest("comment", [] {
			const auto grammarAst = R"(
				# entry point
				main: ;
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(""sv, grammarAst);
			
			EXPECT(std::empty(outputAst.root.nodes));
		});

		addTest("char", [] {
			const auto grammarAst = R"(
				main: 'c';
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammarAst);
			
			EXPECT(std::empty(outputAst.root.nodes));
		});

		addTest("is char", [] {
			const auto grammarAst = R"(
				main: is('c') 'c';
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammarAst);
			
			EXPECT(std::empty(outputAst.root.nodes));
		});

		addTest("escaped chars", [] {
			const auto grammarAst = R"(
				main: "\ \n\t\r\"";
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(" \n\t\r\""sv, grammarAst);
			outputAst.log();
			
			EXPECT(std::empty(outputAst.root.nodes));
		});

		addTest("add char", [] {
			const auto grammarAst = R"(
				main: 'c' add('c');
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammarAst);
			outputAst.log();
			
			EXPECT(std::size(outputAst.root.nodes) == 1);
			EXPECT(outputAst.root.nodes[0].value == 'c');
		});

		addTest("read char", [] {
			const auto grammarAst = R"(
				main: read()+;
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammarAst);
			outputAst.log();
			
			EXPECT(std::size(outputAst.root.nodes) == 1);
			EXPECT(outputAst.root.nodes[0].value == 'c');
		});

		addTest("token", [] {
			const auto grammarAst = R"(
				main: c;
				c: 'c';
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammarAst);
			outputAst.log();
			
			EXPECT(std::empty(outputAst.root.nodes));
		});

		addTest("string", [] {
			const auto grammarAst = R"(
				main: word;
				word: "Hello World!";
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("Hello World!"sv, grammarAst);
			outputAst.log();
			
			EXPECT(std::empty(outputAst.root.nodes));
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
			
			EXPECT(std::size(outputAst.root.nodes) == 3);
			for (auto i = std::size_t{0}; i < 3; ++i)
				EXPECT(outputAst.root.nodes[i].value == 'A');
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
			
			EXPECT(std::size(outputAst.root.nodes) == 3);
			for (auto i = std::size_t{0}; i < 3; ++i)
				EXPECT(outputAst.root.nodes[i].value == 'A');
		});

		addTest("or", [] {
			const auto grammarAst = R"(
				main: or('A', 'B') read()+;
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("BC"sv, grammarAst);
			outputAst.log();
			
			EXPECT(std::size(outputAst.root.nodes) == 1);
			EXPECT(outputAst.root.nodes[0].value == 'C');
		});

		addTest("parenthesis", [] {
			const auto grammarAst = R"(
				main: ('A' 'B') read()+;
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("ABC"sv, grammarAst);
			outputAst.log();
			
			EXPECT(std::size(outputAst.root.nodes) == 1);
			EXPECT(outputAst.root.nodes[0].value == 'C');
		});

		addTest("comparison", [] {
			const auto grammarAst = R"(
				main: char;
				char: or(min, maj);
				min: ['a', 'z']+ add(min);
				maj: ['A', 'Z']+ add(maj);
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("C"sv, grammarAst);
			outputAst.log();
			
			EXPECT(std::size(outputAst.root.nodes) == 1);
			EXPECT(outputAst.root.nodes[0].value == "maj"_token);
		});

		addTest("hash", [] {
			const auto grammarAst = R"(
				main: hash(keyword);
				keyword: read()+ ~keyword;
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("variable"sv, grammarAst);
			outputAst.log();
			
			EXPECT(std::size(outputAst.root.nodes) == 1);
			EXPECT(outputAst.root.nodes[0].value == "variable"_token);
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
			
			EXPECT(std::size(outputAst.root.nodes) == 5);
			EXPECT(outputAst.root.nodes[0].value == 'H');
			EXPECT(outputAst.root.nodes[1].value == 'e');
			EXPECT(outputAst.root.nodes[2].value == 'l');
			EXPECT(outputAst.root.nodes[3].value == 'l');
			EXPECT(outputAst.root.nodes[4].value == 'o');
		});

		addTest("sub", [] {
			const auto grammarAst = R"(
				main: node add(';');
				node: read()+ ~sub(node);
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("ABC"sv, grammarAst);
			outputAst.log();
			
			EXPECT(std::size(outputAst.root.nodes) == 2);
			auto aNode = outputAst.root.nodes[0];
			EXPECT(aNode.value == 'A');
			EXPECT(std::size(aNode.nodes) == 1);
			auto bNode = aNode.nodes[0];
			EXPECT(bNode.value == 'B');
			EXPECT(std::size(bNode.nodes) == 1);
			auto cNode = bNode.nodes[0];
			EXPECT(cNode.value == 'C');
			EXPECT(std::empty(cNode.nodes));
			EXPECT(outputAst.root.nodes[1].value == ';');
		});

		addTest("repeat", [] {
			const auto grammarAst = R"(
				main: repeat('A') repeat('B', ',') read()+;
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("AAAB,B,BC"sv, grammarAst);
			outputAst.log();
			
			EXPECT(std::size(outputAst.root.nodes) == 1);
			EXPECT(outputAst.root.nodes[0].value == 'C');
		});

		addTest("push-pop", [] {
			const auto grammarAst = R"(
				main: push(hash(keyword)) pop();
				keyword: read()+ ~keyword;
			)"_grammar;
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("ABC"sv, grammarAst);
			outputAst.log();

			EXPECT(std::size(outputAst.root.nodes) == 1);
			EXPECT(outputAst.root.nodes[0].value == "ABC"_token);
		});
	}
}
