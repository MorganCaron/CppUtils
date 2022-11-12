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
			CppUtils::Language::Json::jsonGrammarAst.log();
			
			const auto jsonAst = R"(
				{}
			)"_json;
			jsonAst.log();
			
			TEST_ASSERT(std::empty(jsonAst.root.nodes));
		});

		addTest("null", [] {
			const auto jsonAst = R"(
				{
					"key": null
				}
			)"_json;
			jsonAst.log();
			
			TEST_ASSERT(std::size(jsonAst.root.nodes) == 1);
			{
				const auto& nullNode = jsonAst.root.nodes[0];
				TEST_ASSERT(nullNode.value == "key"_token);
				TEST_ASSERT(nullNode.nodes[0].value == "null"_token);
			}
		});

		addTest("boolean", [] {
			const auto jsonAst = R"(
				{
					"bool0": true,
					"bool1": false
				}
			)"_json;
			jsonAst.log();
			
			TEST_ASSERT(std::size(jsonAst.root.nodes) == 2);
			{
				const auto& key0Node = jsonAst.root.nodes[0];
				TEST_ASSERT(key0Node.value == "bool0"_token);
				TEST_ASSERT(std::size(key0Node.nodes) == 1);
				TEST_ASSERT(key0Node.nodes[0].value == "true"_token);
			}
			{
				const auto& key1Node = jsonAst.root.nodes[1];
				TEST_ASSERT(key1Node.value == "bool1"_token);
				TEST_ASSERT(std::size(key1Node.nodes) == 1);
				TEST_ASSERT(key1Node.nodes[0].value == "false"_token);
			}
		});

		addTest("number", [] {
			const auto jsonAst = R"(
				{
					"number0": 42,
					"number1": -21,
					"number2": 3.141592
				}
			)"_json;
			jsonAst.log();
			
			TEST_ASSERT(std::size(jsonAst.root.nodes) == 3);
			{
				const auto& key0Node = jsonAst.root.nodes[0];
				TEST_ASSERT(key0Node.value == "number0"_token);
				TEST_ASSERT(std::size(key0Node.nodes) == 1);
				{
					const auto& number0Node = key0Node.nodes[0];
					TEST_ASSERT(number0Node.value == "number"_token);
					TEST_ASSERT(CppUtils::Language::Parser::getString(number0Node) == "42");
				}
			}
			{
				const auto& key1Node = jsonAst.root.nodes[1];
				TEST_ASSERT(key1Node.value == "number1"_token);
				TEST_ASSERT(std::size(key1Node.nodes) == 1);
				{
					const auto& number1Node = key1Node.nodes[0];
					TEST_ASSERT(number1Node.value == "number"_token);
					TEST_ASSERT(CppUtils::Language::Parser::getString(number1Node) == "-21");
				}
			}
			{
				const auto& key2Node = jsonAst.root.nodes[2];
				TEST_ASSERT(key2Node.value == "number2"_token);
				TEST_ASSERT(std::size(key2Node.nodes) == 1);
				{
					const auto& number2Node = key2Node.nodes[0];
					TEST_ASSERT(number2Node.value == "number"_token);
					TEST_ASSERT(CppUtils::Language::Parser::getString(number2Node) == "3.141592");
				}
			}
		});

		addTest("string", [] {
			const auto jsonAst = R"(
				{
					"key": "hello"
				}
			)"_json;
			jsonAst.log();
			
			TEST_ASSERT(std::size(jsonAst.root.nodes) == 1);
			{
				const auto& keyNode = jsonAst.root.nodes[0];
				TEST_ASSERT(keyNode.value == "key"_token);
				TEST_ASSERT(std::size(keyNode.nodes) == 1);
				{
					const auto& stringNode = keyNode.nodes[0];
					TEST_ASSERT(stringNode.value == "string"_token);
					TEST_ASSERT(CppUtils::Language::Parser::getString(stringNode) == "hello");
				}
			}
		});

		addTest("array", [] {
			const auto jsonAst = R"(
				{
					"key": [1, "abc", true]
				}
			)"_json;
			jsonAst.log();
			
			TEST_ASSERT(std::size(jsonAst.root.nodes) == 1);
			{
				const auto& keyNode = jsonAst.root.nodes[0];
				TEST_ASSERT(keyNode.value == "key"_token);
				TEST_ASSERT(std::size(keyNode.nodes) == 1);
				{
					const auto& arrayNode = keyNode.nodes[0];
					TEST_ASSERT(arrayNode.value == "array"_token);
					TEST_ASSERT(std::size(arrayNode.nodes) == 3);
					{
						const auto& numberNode = arrayNode.nodes[0];
						TEST_ASSERT(numberNode.nodes[0].value == '1');
					}
					{
						const auto& stringNode = arrayNode.nodes[1];
						TEST_ASSERT(stringNode.value == "string"_token);
						TEST_ASSERT(CppUtils::Language::Parser::getString(stringNode) == "abc");
					}
					TEST_ASSERT(arrayNode.nodes[2].value == "true"_token);
				}
			}
		});

		addTest("object", [] {
			const auto jsonAst = R"(
				{
					"key": {
						"number0": 0,
						"number1": 1
					}
				}
			)"_json;
			jsonAst.log();
			
			TEST_ASSERT(std::size(jsonAst.root.nodes) == 1);
			{
				const auto& keyNode = jsonAst.root.nodes[0];
				TEST_ASSERT(keyNode.value == "key"_token);
				TEST_ASSERT(std::size(keyNode.nodes) == 1);
				{
					const auto& objectNode = keyNode.nodes[0];
					TEST_ASSERT(objectNode.value == "object"_token);
					TEST_ASSERT(std::size(objectNode.nodes) == 2);
					{
						const auto& key0Node = objectNode.nodes[0];
						TEST_ASSERT(key0Node.value == "number0"_token);
						TEST_ASSERT(std::size(key0Node.nodes) == 1);
						{
							const auto& numberNode = key0Node.nodes[0];
							TEST_ASSERT(numberNode.value == "number"_token);
							TEST_ASSERT(std::size(numberNode.nodes) == 1);
							TEST_ASSERT(numberNode.nodes[0].value == '0');
						}
					}
					{
						const auto& key1Node = objectNode.nodes[1];
						TEST_ASSERT(key1Node.value == "number1"_token);
						TEST_ASSERT(std::size(key1Node.nodes) == 1);
						{
							const auto& numberNode = key1Node.nodes[0];
							TEST_ASSERT(numberNode.value == "number"_token);
							TEST_ASSERT(std::size(numberNode.nodes) == 1);
							TEST_ASSERT(numberNode.nodes[0].value == '1');
						}
					}
				}
			}
		});
	}
}
