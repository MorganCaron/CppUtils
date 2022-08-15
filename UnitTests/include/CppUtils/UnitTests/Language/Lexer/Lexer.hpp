#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Lexer
{
	TEST_GROUP("Language/Lexer")
	{
		using namespace std::literals;
		using namespace CppUtils::Hash::Literals;

		addTest("base", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{}
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(""sv, grammar);
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
				char_{ string_{A} }
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse("AAAAA"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 0);
		});

		addTest("optional", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ token_{word_} optional_{token_{spaces_}} }
				word_{ optional_{token_{spaces_}} string_{Hello\ World!} }
				spaces_{ token_{space_} optional_{token_{spaces_}} }
				space_{ string_{\ } } space_{ string_{\n} } space_{ string_{\t} } space_{ string_{\r} }
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(R"(
				Hello World!
			)"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 0);
		});

		addTest("comparison", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ optional_{token_{spaces_}} token_{keyword_} optional_{token_{spaces_}} }
				keyword_{ token_{char_} optional_{token_{keyword_}} }
				char_{ >=_{a} <=_{z} + } char_{ >=_{A} <=_{Z} + }
				spaces_{ token_{space_} optional_{token_{spaces_}} }
				space_{ string_{\ } } space_{ string_{\n} } space_{ string_{\t} } space_{ string_{\r} }
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(R"(
				Hello
			)"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 0);
		});

		addTest("read", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ optional_{token_{spaces_}} token_{keyword_} optional_{token_{spaces_}} }
				keyword_{ token_{char_} optional_{token_{keyword_}} }
				char_{ >=_{a} <=_{z} read_+ } char_{ >=_{A} <=_{Z} read_+ }
				spaces_{ token_{space_} optional_{token_{spaces_}} }
				space_{ string_{\ } } space_{ string_{\n} } space_{ string_{\t} } space_{ string_{\r} }
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(R"(
				Hello
			)"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 5);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'H');
			TEST_ASSERT(outputAst.root.nodes[1].value == 'e');
			TEST_ASSERT(outputAst.root.nodes[2].value == 'l');
			TEST_ASSERT(outputAst.root.nodes[3].value == 'l');
			TEST_ASSERT(outputAst.root.nodes[4].value == 'o');
		});

		addTest("add", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ add_{token_{char}} }
			)"sv);
			const auto outputAst = CppUtils::Language::Lexer::parse(""sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == "char"_token);
		});

		addTest("or", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ or_{ string_{A} string_{B} } string_{C} }
			)"sv);
			const auto outputAst = CppUtils::Language::Lexer::parse("BC"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 0);
		});

		addTest("hash", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ optional_{token_{spaces_}} hash_{token_{keyword_}} optional_{token_{spaces_}} }
				keyword_{ token_{char_} optional_{token_{keyword_}} }
				char_{ >=_{a} <=_{z} read_+ } char_{ >=_{A} <=_{Z} read_+ }
				spaces_{ token_{space_} optional_{token_{spaces_}} }
				space_{ string_{\ } } space_{ string_{\n} } space_{ string_{\t} } space_{ string_{\r} }
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(R"(
				variable
			)"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 1);
			TEST_ASSERT(outputAst.root.nodes[0].value == "variable"_token);
		});

		addTest("not", [] {
			const auto grammar = CppUtils::Language::Parser::parseAst(R"(
				main_{ optional_{token_{spaces_}} token_{quote_} optional_{token_{spaces_}} }
				quote_{ string_{"} optional_{token_{quoteContent_}} string_{"} }
				quoteContent_{ not_{string_{"}} read_+ optional_{token_{quoteContent_}} }
				spaces_{ token_{space_} optional_{token_{spaces_}} }
				space_{ string_{\ } } space_{ string_{\n} } space_{ string_{\t} } space_{ string_{\r} }
			)"sv);
			grammar.log();
			const auto outputAst = CppUtils::Language::Lexer::parse(R"(
				"Hello"
			)"sv, grammar);
			outputAst.log();
			TEST_ASSERT(std::size(outputAst.root.nodes) == 5);
			TEST_ASSERT(outputAst.root.nodes[0].value == 'H');
			TEST_ASSERT(outputAst.root.nodes[1].value == 'e');
			TEST_ASSERT(outputAst.root.nodes[2].value == 'l');
			TEST_ASSERT(outputAst.root.nodes[3].value == 'l');
			TEST_ASSERT(outputAst.root.nodes[4].value == 'o');
		});
	}
}
