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
			CppUtils::Language::Markdown::getMarkdownGrammarAst().log();

			const auto markdownAst = R"(
			# Hello World!
			)"_markdown;
			markdownAst.log();

			TEST_ASSERT(std::size(markdownAst.root.nodes) == 1);
			{
				const auto& h1Node = markdownAst.root.nodes[0];
				TEST_ASSERT(h1Node.value == "h1"_token);
				TEST_ASSERT(std::size(h1Node.nodes) == 1);
				{
					const auto& contentNode = h1Node.nodes[0];
					TEST_ASSERT(contentNode.value == "content"_token);
					TEST_ASSERT(std::size(contentNode.nodes) == 1);
					{
						const auto& valueNode = contentNode.nodes[0];
						TEST_ASSERT(valueNode.value == "Hello World!"_token);
					}
				}
			}
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

			TEST_ASSERT(std::size(markdownAst.root.nodes) == 9);
			TEST_ASSERT(std::size(markdownAst.root.nodes[0].nodes) == 1);
			TEST_ASSERT(markdownAst.root.nodes[0].value == "italic"_token);
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

			TEST_ASSERT(markdownAst.root.exists("h1"_token));
			TEST_ASSERT(markdownAst.root.exists("h2"_token));
			TEST_ASSERT(markdownAst.root.exists("h3"_token));
			TEST_ASSERT(markdownAst.root.exists("h4"_token));
			TEST_ASSERT(markdownAst.root.exists("h5"_token));
			TEST_ASSERT(markdownAst.root.exists("h6"_token));
		});

		addTest("Checkbox", [] {
			const auto markdownAst = R"(
			[ ]
			[x]
			)"_markdown;
			markdownAst.log();

			TEST_ASSERT(markdownAst.root.nodes[0].value == "checkbox"_token);
			TEST_ASSERT(markdownAst.root.nodes[1].value == "checkbox"_token);
			TEST_ASSERT(markdownAst.root.nodes.at(0).nodes.empty());
			TEST_ASSERT(markdownAst.root.nodes.at(1).exists("attributes"_token));
			TEST_ASSERT(markdownAst.root.nodes.at(1).nodes.at("attributes"_token).nodes[0].value == "checked"_token);
		});

		addTest("Image", [] {
			const auto markdownAst = R"(
			![title](url)
			)"_markdown;
			markdownAst.log();

			TEST_ASSERT(markdownAst.root.exists("image"_token));
			TEST_ASSERT(markdownAst.root.nodes.at("image"_token).exists("attributes"_token));
			const auto& attributes = markdownAst.root.at("image"_token).at("attributes"_token);
			TEST_ASSERT(attributes.at("alt"_token).nodes[0].value == "title"s);
			TEST_ASSERT(attributes.at("src"_token).nodes[0].value == "url"s);
		});

		addTest("Link", [] {
			const auto markdownAst = R"(
			[title](url)
			)"_markdown;
			markdownAst.log();

			TEST_ASSERT(markdownAst.root.exists("link"_token));
			TEST_ASSERT(markdownAst.root.at("link"_token).exists("attributes"_token));
			const auto& attributes = markdownAst.root.at("link"_token).at("attributes"_token);
			TEST_ASSERT(attributes.at("alt"_token).nodes[0].value == "title"s);
			TEST_ASSERT(attributes.at("src"_token).nodes[0].value == "url"s);
		});

		addTest("Code", [] {
			const auto markdownAst = R"(
			`const variable = 42;`
			)"_markdown;
			markdownAst.log();

			TEST_ASSERT(markdownAst.root.exists("code"_token));
			TEST_ASSERT(markdownAst.root.at("code"_token).exists("content"_token));
			TEST_ASSERT(markdownAst.root.at("code"_token).at("content"_token).nodes[0].value == "const variable = 42;"s);
		});

		addTest("Blockcode", [] {
			const auto markdownAst = R"(
			```js
			const variable = 42;
			```
			)"_markdown;
			markdownAst.log();

			TEST_ASSERT(markdownAst.root.exists("blockcode"_token));
			const auto& blockcode = markdownAst.root.at("blockcode"_token);
			TEST_ASSERT(blockcode.exists("attributes"_token));
			TEST_ASSERT(blockcode.at("attributes"_token).exists("lang"_token));
			TEST_ASSERT(blockcode.at("attributes"_token).at("lang"_token).nodes[0].value == "js"_token);
			TEST_ASSERT(blockcode.exists("content"_token));
			TEST_ASSERT(blockcode.at("content"_token).nodes[0].value == "const variable = 42;"s);
		});
	}
}
