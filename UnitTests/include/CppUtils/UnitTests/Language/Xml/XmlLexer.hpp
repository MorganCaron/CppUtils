#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Xml::XmlLexer
{
	TEST_GROUP("Language/Xml/XmlLexer")
	{
		using namespace CppUtils::Type::Literals;
		using namespace CppUtils::Language::Xml::Literals;
		using namespace CppUtils::Language::StringTree::Literals;
		
		addTest("Tag", [] {
			const auto xmlTree = R"(
			<h1>Title</h1>
			)"_xml;
			CppUtils::Graph::logTreeNode(xmlTree);

			const auto stringTree = R"(
			"h1" {
				"content" { "Title" }
			}
			)"_stringTree;
			ASSERT(xmlTree == stringTree);
		});

		addTest("Nested tags", [] {
			const auto xmlTree = R"(
			<ul>
				<li>First</li>
				<li>Second</li>
				<li>Third</li>
			</ul>
			)"_xml;
			CppUtils::Graph::logTreeNode(xmlTree);

			const auto stringTree = R"(
			"ul" {
				"content" {
					"li" {
						"content" { "First" }
					}
					"li" {
						"content" { "Second" }
					}
					"li" {
						"content" { "Third" }
					}
				}
			}
			)"_stringTree;
			ASSERT(xmlTree == stringTree);
		});

		addTest("Attributes", [] {
			const auto xmlTree = R"(
			<h1 color="red">Title</h1>
			)"_xml;
			CppUtils::Graph::logTreeNode(xmlTree);

			const auto stringTree = R"(
			"h1" {
				"attributes" {
					"color" { "red" }
				}
				"content" { "Title" }
			}
			)"_stringTree;
			ASSERT(xmlTree == stringTree);
		});
	}
}
