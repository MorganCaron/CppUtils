#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Markdown::MarkdownLexer
{
	TEST_GROUP("Language/Markdown/MarkdownLexer")
	{
		using namespace std::literals;
		using namespace CppUtils::Hash::Literals;
		using namespace CppUtils::Language::Markdown::Literals;
		
		addTest("Minimalist", [] {
			const auto markdownAst = R"(
			# Hello World!
			)"_markdown;
			markdownAst.log();

			TEST_ASSERT(markdownAst.root.exists("h1"_token));
			TEST_ASSERT(markdownAst.root.at("h1"_token).exists("content"_token));
			TEST_ASSERT(markdownAst.root.at("h1"_token).at("content"_token).getChildValue() == "Hello World!"s);
		});

		addTest("Format", [] {
			const auto markdownAst = R"(
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
			markdownAst.log();

			TEST_ASSERT(markdownAst.nodes.size() == 9);
			TEST_ASSERT(markdownAst.nodes[0].nodes.size() == 1);
			TEST_ASSERT(markdownAst.nodes[0].getChildValue() == "italic"s);
		});

		addTest("Title", [] {
			const auto markdownAst = R"(
			# H1
			## H2
			### H3
			#### H4
			##### H5
			###### H6
			)"_markdown;
			markdownAst.log();

			TEST_ASSERT(markdownAst.exists("h1"_token));
			TEST_ASSERT(markdownAst.exists("h2"_token));
			TEST_ASSERT(markdownAst.exists("h3"_token));
			TEST_ASSERT(markdownAst.exists("h4"_token));
			TEST_ASSERT(markdownAst.exists("h5"_token));
			TEST_ASSERT(markdownAst.exists("h6"_token));
		});

		addTest("Checkbox", [] {
			const auto markdownAst = R"(
			[ ]
			[x]
			)"_markdown;
			markdownAst.log();

			TEST_ASSERT(markdownAst.getChildValue(0) == "checkbox"_token);
			TEST_ASSERT(markdownAst.getChildValue(1) == "checkbox"_token);
			TEST_ASSERT(markdownAst.nodes.at(0).nodes.empty());
			TEST_ASSERT(markdownAst.nodes.at(1).exists("attributes"_token));
			TEST_ASSERT(markdownAst.nodes.at(1).at("attributes"_token).getChildValue() == "checked"_token);
		});

		addTest("Image", [] {
			const auto markdownAst = R"(
			![title](url)
			)"_markdown;
			markdownAst.log();

			TEST_ASSERT(markdownAst.exists("image"_token));
			TEST_ASSERT(markdownAst.at("image"_token).exists("attributes"_token));
			const auto& attributes = markdownAst.at("image"_token).at("attributes"_token);
			TEST_ASSERT(attributes.at("alt"_token).getChildValue() == "title"s);
			TEST_ASSERT(attributes.at("src"_token).getChildValue() == "url"s);
		});

		addTest("Link", [] {
			const auto markdownAst = R"(
			[title](url)
			)"_markdown;
			markdownAst.log();

			TEST_ASSERT(markdownAst.exists("link"_token));
			TEST_ASSERT(markdownAst.at("link"_token).exists("attributes"_token));
			const auto& attributes = markdownAst.at("link"_token).at("attributes"_token);
			TEST_ASSERT(attributes.at("alt"_token).getChildValue() == "title"s);
			TEST_ASSERT(attributes.at("src"_token).getChildValue() == "url"s);
		});

		addTest("Code", [] {
			const auto markdownAst = R"(
			`const variable = 42;`
			)"_markdown;
			markdownAst.log();

			TEST_ASSERT(markdownAst.exists("code"_token));
			TEST_ASSERT(markdownAst.at("code"_token).exists("content"_token));
			TEST_ASSERT(markdownAst.at("code"_token).at("content"_token).getChildValue() == "const variable = 42;"s);
		});

		addTest("Blockcode", [] {
			const auto markdownAst = R"(
			```js
			const variable = 42;
			```
			)"_markdown;
			markdownAst.log();

			TEST_ASSERT(markdownAst.exists("blockcode"_token));
			const auto& blockcode = markdownAst.at("blockcode"_token);
			TEST_ASSERT(blockcode.exists("attributes"_token));
			TEST_ASSERT(blockcode.at("attributes"_token).exists("lang"_token));
			TEST_ASSERT(blockcode.at("attributes"_token).at("lang"_token).getChildValue() == "js"_token);
			TEST_ASSERT(blockcode.exists("content"_token));
			TEST_ASSERT(blockcode.at("content"_token).getChildValue() == "const variable = 42;"s);
		});
	}
}
