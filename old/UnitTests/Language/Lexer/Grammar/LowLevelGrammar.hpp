#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Lexer::Grammar::LowLevelGrammar
{
	TEST_GROUP("Language/Lexer/Grammar/lowLevelGrammar")
	{
		using namespace std::literals;
		using namespace CppUtils::String::Literals;

		addTest("empty grammar", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			lowLevelGrammarAst.log();

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: ;
				)"sv,
				lowLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(""sv, grammarAst);

			EXPECT(std::empty(outputAst.root.nodes));
		});

		addTest("end", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: end();
				)"sv,
				lowLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(""sv, grammarAst);

			EXPECT(std::empty(outputAst.root.nodes));
		});

		addTest("comment", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					# entry point
					main: ;
				)"sv,
				lowLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(""sv, grammarAst);

			EXPECT(std::empty(outputAst.root.nodes));
		});

		addTest("char", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: 'c';
				)"sv,
				lowLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammarAst);

			EXPECT(std::empty(outputAst.root.nodes));
		});

		addTest("is char", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: is('c') 'c';
				)"sv,
				lowLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammarAst);

			EXPECT(std::empty(outputAst.root.nodes));
		});

		addTest("escaped chars", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: "\ \n\t\r\"";
				)"sv,
				lowLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(" \n\t\r\""sv, grammarAst);
			outputAst.log();

			EXPECT(std::empty(outputAst.root.nodes));
		});

		addTest("add char", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: 'c' add('c');
				)"sv,
				lowLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammarAst);
			outputAst.log();

			EXPECT(std::size(outputAst.root.nodes) == 1);
			EXPECT(outputAst.root.nodes[0].value == 'c');
		});

		addTest("read char", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: read()+;
				)"sv,
				lowLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammarAst);
			outputAst.log();

			EXPECT(std::size(outputAst.root.nodes) == 1);
			EXPECT(outputAst.root.nodes[0].value == 'c');
		});

		addTest("token", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: c;
					c: 'c';
				)"sv,
				lowLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammarAst);
			outputAst.log();

			EXPECT(std::empty(outputAst.root.nodes));
		});

		addTest("string", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: word;
					word: "Hello World!";
				)"sv,
				lowLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("Hello World!"sv, grammarAst);
			outputAst.log();

			EXPECT(std::empty(outputAst.root.nodes));
		});

		addTest("recurrence", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: chars;
					chars: char chars; chars: char;
					char: read()+;
				)"sv,
				lowLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("AAA"sv, grammarAst);
			outputAst.log();

			EXPECT(std::size(outputAst.root.nodes) == 3);
			for (auto i = std::size_t{0}; i < 3; ++i) // Todo: ""uz (not supported on MSVC)
				EXPECT(outputAst.root.nodes[i].value == 'A');
		});

		addTest("optional", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: chars;
					chars: char ~chars;
					char: read()+;
				)"sv,
				lowLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("AAA"sv, grammarAst);
			outputAst.log();

			EXPECT(std::size(outputAst.root.nodes) == 3);
			for (auto i = std::size_t{0}; i < 3; ++i) // Todo: ""uz (not supported on MSVC)
				EXPECT(outputAst.root.nodes[i].value == 'A');
		});

		addTest("or", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: or('A', 'B') read()+;
				)"sv,
				lowLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("BC"sv, grammarAst);
			outputAst.log();

			EXPECT(std::size(outputAst.root.nodes) == 1);
			EXPECT(outputAst.root.nodes[0].value == 'C');
		});

		addTest("parenthesis", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: ('A' 'B') read()+;
				)"sv,
				lowLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("ABC"sv, grammarAst);
			outputAst.log();

			EXPECT(std::size(outputAst.root.nodes) == 1);
			EXPECT(outputAst.root.nodes[0].value == 'C');
		});

		addTest("comparison", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: char;
					char: or(min, maj);
					min: ['a', 'z']+ add(min);
					maj: ['A', 'Z']+ add(maj);
				)"sv,
				lowLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("C"sv, grammarAst);
			outputAst.log();

			EXPECT(std::size(outputAst.root.nodes) == 1);
			EXPECT(outputAst.root.nodes[0].value == "maj"_token);
		});

		addTest("hash", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: hash(keyword);
					keyword: read()+ ~keyword;
				)"sv,
				lowLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("variable"sv, grammarAst);
			outputAst.log();

			EXPECT(std::size(outputAst.root.nodes) == 1);
			EXPECT(outputAst.root.nodes[0].value == "variable"_token);
		});

		addTest("not", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: quote;
					quote: '"' ~quoteContent '"';
					quoteContent: !'"' read()+ ~quoteContent;
				)"sv,
				lowLevelGrammarAst);
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
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: node add(';');
					node: read()+ ~sub(node);
				)"sv,
				lowLevelGrammarAst);
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
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: repeat('A') repeat('B', ',') read()+;
				)"sv,
				lowLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("AAAB,B,BC"sv, grammarAst);
			outputAst.log();

			EXPECT(std::size(outputAst.root.nodes) == 1);
			EXPECT(outputAst.root.nodes[0].value == 'C');
		});

		addTest("push-pop", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: push(hash(keyword)) pop();
					keyword: read()+ ~keyword;
				)"sv,
				lowLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("ABC"sv, grammarAst);
			outputAst.log();

			EXPECT(std::size(outputAst.root.nodes) == 1);
			EXPECT(outputAst.root.nodes[0].value == "ABC"_token);
		});

		addTest("unsigned int", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);

			const auto grammarAst = CppUtils::Language::Lexer::parse(R"(
					main: uint;
					uint: add('\0') repeat(digit);
					digit: ['0', '9'] read()+;
				)"sv,
				lowLevelGrammarAst);
			grammarAst.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("42"sv, grammarAst);
			outputAst.log();

			EXPECT(std::size(outputAst.root.nodes) == 1);
			EXPECT(outputAst.root.nodes[0].value == 42);
		});
	}
}
