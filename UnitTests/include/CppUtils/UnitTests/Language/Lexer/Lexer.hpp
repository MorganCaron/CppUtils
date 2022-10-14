#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Lexer
{
	TEST_GROUP("Language/Lexer")
	{
		using namespace std::literals;
		using namespace CppUtils::Hash::Literals;

		addTest("empty grammar", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{}
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(""sv, grammar);
			outputAst.log();

			TEST_ASSERT(std::size(outputAst.root.nodes) == 0);
		});

		addTest("char", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ string_{c} }
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammar);
			outputAst.log();

			TEST_ASSERT(std::size(outputAst.root.nodes) == 0);
		});

		addTest("escaped chars", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ string_{\ } string_{\n} string_{\t} string_{\r} }
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(" \n\t\r"sv, grammar);
			outputAst.log();

			TEST_ASSERT(std::size(outputAst.root.nodes) == 0);
		});

		addTest("add char", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ string_{c} add_{c} }
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammar);
			outputAst.log();

			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'c');
		});
		
		addTest("read char", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ read_+ }
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammar);
			outputAst.log();

			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'c');
		});

		addTest("token", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ token_{c_} }
				c_{ string_{c} }
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("c"sv, grammar);
			outputAst.log();

			TEST_ASSERT(std::size(outputAst.root.nodes) == 0);
		});

		addTest("string", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ token_{word_} }
				word_{ string_{Hello\ World!} }
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("Hello World!"sv, grammar);
			outputAst.log();

			TEST_ASSERT(std::size(outputAst.root.nodes) == 0);
		});

		addTest("recurrence", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ token_{chars_} }
				chars_{ token_{char_} token_{chars_} } chars_{ token_{char_} }
				char_{ read_+ }
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("AAA"sv, grammar);
			outputAst.log();

			TEST_ASSERT(std::size(outputAst.root.nodes) == 3);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'A');
			TEST_ASSERT(outputAst.root.nodes[1].value == 'A');
			TEST_ASSERT(outputAst.root.nodes[2].value == 'A');
		});

		addTest("optional", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ token_{chars_} }
				chars_{ token_{char_} optional_{token_{chars_}} }
				char_{ read_+ }
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("AAA"sv, grammar);
			outputAst.log();

			TEST_ASSERT(std::size(outputAst.root.nodes) == 3);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'A');
			TEST_ASSERT(outputAst.root.nodes[1].value == 'A');
			TEST_ASSERT(outputAst.root.nodes[2].value == 'A');
		});

		addTest("or", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ or_{ string_{A} string_{B} } read_+ }
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("BC"sv, grammar);
			outputAst.log();

			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'C');
		});

		addTest("parenthesis", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ parenthesis_{ string_{A} string_{B} } read_+ }
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("ABC"sv, grammar);
			outputAst.log();

			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'C');
		});

		addTest("comparison", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ token_{char_} }
				char_{ or_{ token_{min_} token_{maj_} } }
				min_{ >=_{a} <=_{z} + add_{min_} }
				maj_{ >=_{A} <=_{Z} + add_{maj_} }
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("C"sv, grammar);
			outputAst.log();

			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == "maj"_token);
		});

		addTest("hash", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ hash_{token_{keyword_}} }
				keyword_{ read_+ optional_{token_{keyword_}} }
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("variable"sv, grammar);
			outputAst.log();

			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == "variable"_token);
		});

		addTest("not", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ token_{quote_} }
				quote_{ string_{"} optional_{token_{quoteContent_}} string_{"} }
				quoteContent_{ not_{string_{"}} read_+ optional_{token_{quoteContent_}} }
			)"sv);
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
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ token_{node_} add_{;} }
				node_{ read_+ optional_{sub_{token_{node_}}} }
			)"sv);
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
