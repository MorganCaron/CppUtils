#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Markdown::MarkdownLexer
{
	TEST_GROUP("Language/Markdown/MarkdownLexer")
	{
		using namespace std::literals;
		using namespace CppUtils::Type::Literals;
		using namespace CppUtils::Language::Markdown::Literals;
		
		addTest("Minimalist", [] {
			const auto markdownTree = R"(
			# Hello World!
			)"_markdown;
			CppUtils::Graph::logTreeNode(markdownTree);

			ASSERT(markdownTree.exists("h1"_token));
			ASSERT(markdownTree.at("h1"_token).exists("content"_token));
			ASSERT(markdownTree.at("h1"_token).at("content"_token).getChildValue() == "Hello World!"s);
		});

		addTest("Format", [] {
			const auto markdownTree = R"(
			*italic*
			_italic_
			**bold**
			__bold__
			*__italic and bold__*
			_**italic and bold**_
			__*italic and bold*__
			**_italic and bold_**
			___italic and bold___
			)"_markdown;
			CppUtils::Graph::logTreeNode(markdownTree);

			ASSERT(markdownTree.childs.size() == 9);
			ASSERT(markdownTree.childs.at(0).childs.size() == 1);
			ASSERT(markdownTree.childs.at(0).getChildValue() == "italic"s);
		});

		addTest("Title", [] {
			const auto markdownTree = R"(
			# H1
			## H2
			### H3
			#### H4
			##### H5
			###### H6
			)"_markdown;
			CppUtils::Graph::logTreeNode(markdownTree);

			ASSERT(markdownTree.exists("h1"_token));
			ASSERT(markdownTree.exists("h2"_token));
			ASSERT(markdownTree.exists("h3"_token));
			ASSERT(markdownTree.exists("h4"_token));
			ASSERT(markdownTree.exists("h5"_token));
			ASSERT(markdownTree.exists("h6"_token));
		});

		addTest("Checkbox", [] {
			const auto markdownTree = R"(
			[ ]
			[x]
			)"_markdown;
			CppUtils::Graph::logTreeNode(markdownTree);

			ASSERT(markdownTree.getChildValue(0) == "checkbox"_token);
			ASSERT(markdownTree.getChildValue(1) == "checkbox"_token);
			ASSERT(markdownTree.childs.at(0).childs.empty());
			ASSERT(markdownTree.childs.at(1).exists("attributes"_token));
			ASSERT(markdownTree.childs.at(1).at("attributes"_token).getChildValue() == "checked"_token);
		});

		addTest("Image", [] {
			const auto markdownTree = R"(
			![title](url)
			)"_markdown;
			CppUtils::Graph::logTreeNode(markdownTree);

			ASSERT(markdownTree.exists("image"_token));
			ASSERT(markdownTree.at("image"_token).exists("attributes"_token));
			const auto& attributes = markdownTree.at("image"_token).at("attributes"_token);
			ASSERT(attributes.at("alt"_token).getChildValue() == "title"s);
			ASSERT(attributes.at("src"_token).getChildValue() == "url"s);
		});

		addTest("Link", [] {
			const auto markdownTree = R"(
			[title](url)
			)"_markdown;
			CppUtils::Graph::logTreeNode(markdownTree);

			ASSERT(markdownTree.exists("link"_token));
			ASSERT(markdownTree.at("link"_token).exists("attributes"_token));
			const auto& attributes = markdownTree.at("link"_token).at("attributes"_token);
			ASSERT(attributes.at("alt"_token).getChildValue() == "title"s);
			ASSERT(attributes.at("src"_token).getChildValue() == "url"s);
		});

		addTest("List", [] {});
		addTest("Table", [] {});

		addTest("Code", [] {
			const auto markdownTree = R"(
			`const variable = 42;`
			)"_markdown;
			CppUtils::Graph::logTreeNode(markdownTree);

			ASSERT(markdownTree.exists("code"_token));
			ASSERT(markdownTree.at("code"_token).exists("content"_token));
			ASSERT(markdownTree.at("code"_token).at("content"_token).getChildValue() == "const variable = 42;"s);
		});

		addTest("Blockcode", [] {
			const auto markdownTree = R"(
			```js
			const variable = 42;
			```
			)"_markdown;
			CppUtils::Graph::logTreeNode(markdownTree);

			ASSERT(markdownTree.exists("blockcode"_token));
			const auto& blockcode = markdownTree.at("blockcode"_token);
			ASSERT(blockcode.exists("attributes"_token));
			ASSERT(blockcode.at("attributes"_token).exists("lang"_token));
			ASSERT(blockcode.at("attributes"_token).at("lang"_token).getChildValue() == "js"_token);
			ASSERT(blockcode.exists("content"_token));
			ASSERT(blockcode.at("content"_token).getChildValue() == "const variable = 42;"s);
		});
	}
}
