#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Json::JsonLexer
{
	const auto tests = std::vector<CppUtils::Test>{

		CppUtils::Test{"Language/Json/JsonLexer/minimalist", [] {
			using namespace CppUtils::Type::Literals;
			using namespace CppUtils::Language::Json::Literals;
			const auto jsonTree = R"(
			{
				"value0": "text",
				"value1": 3.14159
			}
			)"_json;
			CppUtils::Graph::logTreeNode(jsonTree);

			ASSERT(jsonTree.childs.size() == 2);
			ASSERT(jsonTree.childs.at(0).value == "value0"_token);
			ASSERT(jsonTree.childs.at(1).value == "value1"_token);
			ASSERT(jsonTree.childs.at(0).childs.size() == 1);
			ASSERT(jsonTree.childs.at(1).childs.size() == 1);
			ASSERT(jsonTree.childs.at(0).childs.at(0).value == "string"_token);
			ASSERT(jsonTree.childs.at(1).childs.at(0).value == "number"_token);
			ASSERT(jsonTree.childs.at(0).childs.at(0).childs.size() == 1);
			ASSERT(jsonTree.childs.at(1).childs.at(0).childs.size() == 1);
			ASSERT(jsonTree.childs.at(0).childs.at(0).childs.at(0).value == "text"_token);
			ASSERT(jsonTree.childs.at(1).childs.at(0).childs.at(0).value == 3.14159f);
		}},

		CppUtils::Test{"Language/Json/JsonLexer/object", [] {
			using namespace CppUtils::Type::Literals;
			using namespace CppUtils::Language::Json::Literals;
			const auto jsonTree = R"(
			{
				"value0": "text",
				"value1": 3.14159,
				"object0": {
					"value0": "text",
					"value1": 3.14159
				}
			}
			)"_json;
			CppUtils::Graph::logTreeNode(jsonTree);

			ASSERT(jsonTree.childs.size() == 3);
			ASSERT(jsonTree.childs.at(2).value == "object0"_token);
			ASSERT(jsonTree.childs.at(2).childs.size() == 1);
			ASSERT(jsonTree.childs.at(2).childs.at(0).value == "object"_token);
			ASSERT(jsonTree.childs.at(2).childs.at(0).childs.size() == 2);
			ASSERT(jsonTree.childs.at(2).childs.at(0).childs.at(0) == jsonTree.childs.at(0));
			ASSERT(jsonTree.childs.at(2).childs.at(0).childs.at(1) == jsonTree.childs.at(1));
		}},

		CppUtils::Test{"Language/Json/JsonLexer/array", [] {
			using namespace CppUtils::Type::Literals;
			using namespace CppUtils::Language::Json::Literals;
			const auto jsonTree = R"(
			{
				"array0": [0, 1, 2, 3, 4]
			}
			)"_json;
			CppUtils::Graph::logTreeNode(jsonTree);

			ASSERT(jsonTree.childs.size() == 1);
			ASSERT(jsonTree.childs.at(0).value == "array0"_token);
			ASSERT(jsonTree.childs.at(0).childs.size() == 1);
			ASSERT(jsonTree.childs.at(0).childs.at(0).value == "array"_token);
			ASSERT(jsonTree.childs.at(0).childs.at(0).childs.size() == 5);
		}}

	};
}
