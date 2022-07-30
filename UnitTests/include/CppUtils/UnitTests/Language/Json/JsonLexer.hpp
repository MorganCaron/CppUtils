#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Json::JsonLexer
{
	TEST_GROUP("Language/Json/JsonLexer")
	{
		using namespace std::literals;
		using namespace CppUtils::Hash::Literals;
		using namespace CppUtils::Language::Json::Literals;

		addTest("minimalist", [] {
			const auto jsonAst = R"(
			{
				"null": null,
				"boolean": true,
				"number": 3.14159,
				"string": "Hello World!"
			}
			)"_json;
			jsonAst.log();

			TEST_ASSERT(std::size(jsonAst.root.nodes) == 4);
			TEST_ASSERT(jsonAst.root.nodes[0].value == "null"_token);
			TEST_ASSERT(jsonAst.root.nodes[1].value == "boolean"_token);
			TEST_ASSERT(jsonAst.root.nodes[2].value == "number"_token);
			TEST_ASSERT(jsonAst.root.nodes[3].value == "string"_token);
			TEST_ASSERT(jsonAst.root.nodes[0].nodes[0].value == "null"_token);
			TEST_ASSERT(jsonAst.root.nodes[1].nodes[0].value == "true"_token);
			TEST_ASSERT(jsonAst.root.nodes[2].nodes[0].value == 3.14159f);
			// TEST_ASSERT(jsonAst.root.nodes[3].nodes[0].value == "Hello World!"s);
		});

		addTest("object", [] {
			const auto jsonAst = R"(
			{
				"object": {
					"key0": "value0",
					"key1": "value1"
				}
			}
			)"_json;
			jsonAst.log();

			TEST_ASSERT(jsonAst.exists("object"_token));
			const auto& object = jsonAst.at("object"_token);
			TEST_ASSERT(object.exists("key0"_token));
			TEST_ASSERT(object.exists("key1"_token));
			TEST_ASSERT(object.at("key0"_token).getChildValue() == "value0"s);
			TEST_ASSERT(object.at("key1"_token).getChildValue() == "value1"s);
		});

		addTest("array", [] {
			const auto jsonAst = R"(
			{
				"array": [0, 1, 2]
			}
			)"_json;
			jsonAst.log();

			TEST_ASSERT(jsonAst.exists("array"_token));
			const auto& array = jsonAst.at("array"_token);
			TEST_ASSERT(array.childs.size() == 3);
			TEST_ASSERT(array.getChildValue(0) == 0.f);
			TEST_ASSERT(array.getChildValue(1) == 1.f);
			TEST_ASSERT(array.getChildValue(2) == 2.f);
		});
	}
}
