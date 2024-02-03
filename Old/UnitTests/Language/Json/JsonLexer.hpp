#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Json::JsonLexer
{
	TEST_GROUP("Language/Json/JsonLexer")
	{
		using namespace std::literals;
		using namespace CppUtils::String::Literals;
		using namespace CppUtils::Language::Json::Literals;

		addTest("empty json", [] {
			CppUtils::Language::Json::getJsonGrammarAst().log();

			const auto jsonAst = R"(
				{}
			)"_json;
			jsonAst.log();

			EXPECT(std::empty(jsonAst.root.nodes));
		});

		addTest("null", [] {
			const auto jsonAst = R"(
				{
					"key": null
				}
			)"_json;
			jsonAst.log();

			EXPECT(std::size(jsonAst.root.nodes) == 1);
			{
				const auto& nullNode = jsonAst.root.nodes[0];
				EXPECT(nullNode.value == "key"_token);
				EXPECT(nullNode.nodes[0].value == "null"_token);
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

			EXPECT(std::size(jsonAst.root.nodes) == 2);
			{
				const auto& key0Node = jsonAst.root.nodes[0];
				EXPECT(key0Node.value == "bool0"_token);
				EXPECT(std::size(key0Node.nodes) == 1);
				EXPECT(key0Node.nodes[0].value == "true"_token);
			}
			{
				const auto& key1Node = jsonAst.root.nodes[1];
				EXPECT(key1Node.value == "bool1"_token);
				EXPECT(std::size(key1Node.nodes) == 1);
				EXPECT(key1Node.nodes[0].value == "false"_token);
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

			EXPECT(std::size(jsonAst.root.nodes) == 3);
			{
				const auto& key0Node = jsonAst.root.nodes[0];
				EXPECT(key0Node.value == "number0"_token);
				EXPECT(std::size(key0Node.nodes) == 1);
				{
					const auto& number0Node = key0Node.nodes[0];
					EXPECT(number0Node.value == "number"_token);
					EXPECT(std::size(number0Node.nodes) == 1);
					{
						const auto& valueNode = number0Node.nodes[0];
						EXPECT(valueNode.value == "42"_token);
					}
				}
			}
			{
				const auto& key1Node = jsonAst.root.nodes[1];
				EXPECT(key1Node.value == "number1"_token);
				EXPECT(std::size(key1Node.nodes) == 1);
				{
					const auto& number1Node = key1Node.nodes[0];
					EXPECT(number1Node.value == "number"_token);
					EXPECT(std::size(number1Node.nodes) == 1);
					{
						const auto& valueNode = number1Node.nodes[0];
						EXPECT(valueNode.value == "-21"_token);
					}
				}
			}
			{
				const auto& key2Node = jsonAst.root.nodes[2];
				EXPECT(key2Node.value == "number2"_token);
				EXPECT(std::size(key2Node.nodes) == 1);
				{
					const auto& number2Node = key2Node.nodes[0];
					EXPECT(number2Node.value == "number"_token);
					EXPECT(std::size(number2Node.nodes) == 1);
					{
						const auto& valueNode = number2Node.nodes[0];
						EXPECT(valueNode.value == "3.141592"_token);
					}
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

			EXPECT(std::size(jsonAst.root.nodes) == 1);
			{
				const auto& keyNode = jsonAst.root.nodes[0];
				EXPECT(keyNode.value == "key"_token);
				EXPECT(std::size(keyNode.nodes) == 1);
				{
					const auto& stringNode = keyNode.nodes[0];
					EXPECT(stringNode.value == "string"_token);
					EXPECT(std::size(stringNode.nodes) == 1);
					{
						const auto& valueNode = stringNode.nodes[0];
						EXPECT(valueNode.value == "hello"_token);
					}
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

			EXPECT(std::size(jsonAst.root.nodes) == 1);
			{
				const auto& keyNode = jsonAst.root.nodes[0];
				EXPECT(keyNode.value == "key"_token);
				EXPECT(std::size(keyNode.nodes) == 1);
				{
					const auto& arrayNode = keyNode.nodes[0];
					EXPECT(arrayNode.value == "array"_token);
					EXPECT(std::size(arrayNode.nodes) == 3);
					{
						const auto& numberNode = arrayNode.nodes[0];
						EXPECT(numberNode.nodes[0].value == "1"_token);
					}
					{
						const auto& stringNode = arrayNode.nodes[1];
						EXPECT(stringNode.value == "string"_token);
						EXPECT(std::size(stringNode.nodes) == 1);
						EXPECT(stringNode.nodes[0].value == "abc"_token);
					}
					EXPECT(arrayNode.nodes[2].value == "true"_token);
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

			EXPECT(std::size(jsonAst.root.nodes) == 1);
			{
				const auto& keyNode = jsonAst.root.nodes[0];
				EXPECT(keyNode.value == "key"_token);
				EXPECT(std::size(keyNode.nodes) == 1);
				{
					const auto& objectNode = keyNode.nodes[0];
					EXPECT(objectNode.value == "object"_token);
					EXPECT(std::size(objectNode.nodes) == 2);
					{
						const auto& key0Node = objectNode.nodes[0];
						EXPECT(key0Node.value == "number0"_token);
						EXPECT(std::size(key0Node.nodes) == 1);
						{
							const auto& numberNode = key0Node.nodes[0];
							EXPECT(numberNode.value == "number"_token);
							EXPECT(std::size(numberNode.nodes) == 1);
							EXPECT(numberNode.nodes[0].value == "0"_token);
						}
					}
					{
						const auto& key1Node = objectNode.nodes[1];
						EXPECT(key1Node.value == "number1"_token);
						EXPECT(std::size(key1Node.nodes) == 1);
						{
							const auto& numberNode = key1Node.nodes[0];
							EXPECT(numberNode.value == "number"_token);
							EXPECT(std::size(numberNode.nodes) == 1);
							EXPECT(numberNode.nodes[0].value == "1"_token);
						}
					}
				}
			}
		});
	}
}
