#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Json::JsonLexer
{
	TEST_GROUP("Language/Json/JsonLexer")
	{
		using namespace std::literals;
		using namespace CppUtils::Hash::Literals;
		using namespace CppUtils::Language::Json::Literals;

		addTest("empty json", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			const auto jsonGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Json::jsonGrammarSrc, highLevelGrammarAst);
			jsonGrammarAst.log();
			
			const auto jsonAst = CppUtils::Language::Lexer::parse(R"(
				{}
			)"sv, jsonGrammarAst);
			jsonAst.log();
			
			TEST_ASSERT(std::empty(jsonAst.root.nodes));
		});

		addTest("null", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			const auto jsonGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Json::jsonGrammarSrc, highLevelGrammarAst);
			
			const auto jsonAst = CppUtils::Language::Lexer::parse(R"(
				{
					"null": null
				}
			)"sv, jsonGrammarAst);
			jsonAst.log();
			
			TEST_ASSERT(std::size(jsonAst.root.nodes) == 1);
			{
				const auto& booleanNode = jsonAst.root.nodes[0];
				TEST_ASSERT(booleanNode.value == "null"_token);
				TEST_ASSERT(booleanNode.nodes[0].value == "null"_token);
			}
		});

		addTest("boolean", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			const auto jsonGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Json::jsonGrammarSrc, highLevelGrammarAst);
			
			const auto jsonAst = CppUtils::Language::Lexer::parse(R"(
				{
					"true": true,
					"false": false
				}
			)"sv, jsonGrammarAst);
			jsonAst.log();
			
			TEST_ASSERT(std::size(jsonAst.root.nodes) == 2);
			{
				const auto& true0Node = jsonAst.root.nodes[0];
				TEST_ASSERT(true0Node.value == "true"_token);
				TEST_ASSERT(std::size(true0Node.nodes) == 1);
				TEST_ASSERT(true0Node.nodes[0].value == "true"_token);
			}
			{
				const auto& false0Node = jsonAst.root.nodes[1];
				TEST_ASSERT(false0Node.value == "false"_token);
				TEST_ASSERT(std::size(false0Node.nodes) == 1);
				TEST_ASSERT(false0Node.nodes[0].value == "false"_token);
			}
		});

		addTest("number", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			const auto jsonGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Json::jsonGrammarSrc, highLevelGrammarAst);
			
			const auto jsonAst = CppUtils::Language::Lexer::parse(R"(
				{
					"number0": 42,
					"number1": -21,
					"number2": 3.141592
				}
			)"sv, jsonGrammarAst);
			jsonAst.log();
			
			TEST_ASSERT(std::size(jsonAst.root.nodes) == 3);
			{
				const auto& number0Node = jsonAst.root.nodes[0];
				TEST_ASSERT(number0Node.value == "number0"_token);
				TEST_ASSERT(std::size(number0Node.nodes) == 2);
				TEST_ASSERT(number0Node.nodes[0].value == '4');
				TEST_ASSERT(number0Node.nodes[1].value == '2');
			}
			{
				const auto& number1Node = jsonAst.root.nodes[1];
				TEST_ASSERT(number1Node.value == "number1"_token);
				TEST_ASSERT(std::size(number1Node.nodes) == 3);
				TEST_ASSERT(number1Node.nodes[0].value == '-');
				TEST_ASSERT(number1Node.nodes[1].value == '2');
				TEST_ASSERT(number1Node.nodes[2].value == '1');
			}
			{
				const auto& number2Node = jsonAst.root.nodes[2];
				TEST_ASSERT(number2Node.value == "number2"_token);
				TEST_ASSERT(std::size(number2Node.nodes) == 8);
				TEST_ASSERT(number2Node.nodes[0].value == '3');
				TEST_ASSERT(number2Node.nodes[1].value == '.');
				TEST_ASSERT(number2Node.nodes[2].value == '1');
				TEST_ASSERT(number2Node.nodes[3].value == '4');
				TEST_ASSERT(number2Node.nodes[4].value == '1');
				TEST_ASSERT(number2Node.nodes[5].value == '5');
				TEST_ASSERT(number2Node.nodes[6].value == '9');
				TEST_ASSERT(number2Node.nodes[7].value == '2');
			}
		});

		addTest("string", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			const auto jsonGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Json::jsonGrammarSrc, highLevelGrammarAst);
			
			const auto jsonAst = CppUtils::Language::Lexer::parse(R"(
				{
					"string": "hello"
				}
			)"sv, jsonGrammarAst);
			jsonAst.log();
			
			TEST_ASSERT(std::size(jsonAst.root.nodes) == 1);
			{
				const auto& stringNode = jsonAst.root.nodes[0];
				TEST_ASSERT(stringNode.value == "string"_token);
				TEST_ASSERT(std::size(stringNode.nodes) == 5);
				TEST_ASSERT(stringNode.nodes[0].value == 'h');
				TEST_ASSERT(stringNode.nodes[1].value == 'e');
				TEST_ASSERT(stringNode.nodes[2].value == 'l');
				TEST_ASSERT(stringNode.nodes[3].value == 'l');
				TEST_ASSERT(stringNode.nodes[4].value == 'o');
			}
		});

		addTest("array", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			const auto jsonGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Json::jsonGrammarSrc, highLevelGrammarAst);
			
			const auto jsonAst = CppUtils::Language::Lexer::parse(R"(
				{
					"array": [1, 'a', true]
				}
			)"sv, jsonGrammarAst);
			jsonAst.log();
			
			TEST_ASSERT(std::size(jsonAst.root.nodes) == 1);
			{
				const auto& arrayNode = jsonAst.root.nodes[0];
				TEST_ASSERT(arrayNode.value == "array"_token);
				TEST_ASSERT(std::size(arrayNode.nodes) == 3);
				TEST_ASSERT(arrayNode.nodes[0].value == '1');
				TEST_ASSERT(arrayNode.nodes[1].value == 'a');
				TEST_ASSERT(arrayNode.nodes[2].value == "true"_token);
			}
		});

		addTest("object", [] {
			const auto lowLevelGrammarAst = CppUtils::Language::Parser::parseAst(CppUtils::Language::Lexer::Grammar::lowLevelGrammarSrc);
			const auto highLevelGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Lexer::Grammar::highLevelGrammarSrc, lowLevelGrammarAst);
			const auto jsonGrammarAst = CppUtils::Language::Lexer::parse(CppUtils::Language::Json::jsonGrammarSrc, highLevelGrammarAst);
			
			const auto jsonAst = CppUtils::Language::Lexer::parse(R"(
				{
					"object": {
						"key0": 0,
						"key1": 1
					}
				}
			)"sv, jsonGrammarAst);
			jsonAst.log();
			
			TEST_ASSERT(std::size(jsonAst.root.nodes) == 1);
			{
				const auto& objectNode = jsonAst.root.nodes[0];
				TEST_ASSERT(objectNode.value == "object"_token);
				TEST_ASSERT(std::size(objectNode.nodes) == 2);
				{
					const auto& key0Node = objectNode.nodes[0];
					TEST_ASSERT(key0Node.value == "key0"_token);
					TEST_ASSERT(std::size(key0Node.nodes) == 1);
					TEST_ASSERT(key0Node.nodes[0].value == '0');
				}
				{
					const auto& key1Node = objectNode.nodes[1];
					TEST_ASSERT(key1Node.value == "key1"_token);
					TEST_ASSERT(std::size(key1Node.nodes) == 1);
					TEST_ASSERT(key1Node.nodes[0].value == '1');
				}
			}
		});
	}
}
