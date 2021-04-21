#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Json::JsonLexer
{
	TEST_GROUP("Language/Json/JsonLexer")
	{
		using namespace CppUtils::Type::Literals;
		using namespace CppUtils::Language::Json::Literals;
		
		addTest("minimalist", [] {
			const auto jsonTree = R"(
			{
				"null": null,
				"boolean": true,
				"number": 3.14159,
				"string": "Hello World!"
			}
			)"_json;
			CppUtils::Graph::logTreeNode(jsonTree);

			ASSERT(jsonTree.exists("null"_token));
			ASSERT(jsonTree.exists("boolean"_token));
			ASSERT(jsonTree.exists("number"_token));
			ASSERT(jsonTree.exists("string"_token));
			ASSERT(jsonTree.at("null"_token).getChildValue() == "null"_token);
			ASSERT(jsonTree.at("boolean"_token).getChildValue() == true);
			ASSERT(jsonTree.at("number"_token).getChildValue() == 3.14159f);
			ASSERT(jsonTree.at("string"_token).getChildValue() == "Hello World!"_token);
		});

		addTest("object", [] {
			const auto jsonTree = R"(
			{
				"object": {
					"key0": "value0",
					"key1": "value1"
				}
			}
			)"_json;
			CppUtils::Graph::logTreeNode(jsonTree);

			ASSERT(jsonTree.exists("object"_token));
			const auto& object = jsonTree.at("object"_token);
			ASSERT(object.exists("key0"_token));
			ASSERT(object.exists("key1"_token));
			ASSERT(object.at("key0"_token).getChildValue() == "value0"_token);
			ASSERT(object.at("key1"_token).getChildValue() == "value1"_token);
		});

		addTest("array", [] {
			const auto jsonTree = R"(
			{
				"array": [0, 1, 2]
			}
			)"_json;
			CppUtils::Graph::logTreeNode(jsonTree);

			ASSERT(jsonTree.exists("array"_token));
			const auto& array = jsonTree.at("array"_token);
			ASSERT(array.childs.size() == 3);
			ASSERT(array.getChildValue(0) == 0.f);
			ASSERT(array.getChildValue(1) == 1.f);
			ASSERT(array.getChildValue(2) == 2.f);
		});
	}
}
