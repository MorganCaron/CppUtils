#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Xml::XmlLexer
{
	TEST_GROUP("Language/Xml/XmlLexer")
	{
		using namespace std::literals;
		using namespace CppUtils::Type::Literals;
		using namespace CppUtils::Language::Xml::Literals;
		
		addTest("Tag", [] {
			const auto xmlTree = R"(
			<h1>Title</h1>
			)"_xml;
			CppUtils::Graph::logTreeNode(xmlTree);

			ASSERT(xmlTree.childs.size() == 1);
			ASSERT(xmlTree.childs.at(0).value == "h1"_token);
			ASSERT(xmlTree.childs.at(0).childs.size() == 2);
			ASSERT(xmlTree.childs.at(0).childs.at(0).value == "attributes"_token);
			ASSERT(xmlTree.childs.at(0).childs.at(1).value == "Title"s);
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

			ASSERT(xmlTree.childs.size() == 1);
			ASSERT(xmlTree.childs.at(0).value == "ul"_token);
			ASSERT(xmlTree.childs.at(0).childs.size() == 4);
			ASSERT(xmlTree.childs.at(0).childs.at(0).value == "attributes"_token);
			for (auto i = 1u; i <= 3; ++i)
				ASSERT(xmlTree.childs.at(0).childs.at(i).value == "li"_token);
			ASSERT(xmlTree.childs.at(0).childs.at(1).childs.size() == 2);
			ASSERT(xmlTree.childs.at(0).childs.at(1).childs.at(0).value == "attributes"_token);
			ASSERT(xmlTree.childs.at(0).childs.at(1).childs.at(1).value == "First"s);
		});

		addTest("Attributes", [] {
			const auto xmlTree = R"(
			<h1 color="red">Title</h1>
			)"_xml;
			CppUtils::Graph::logTreeNode(xmlTree);

			ASSERT(xmlTree.childs.size() == 1);
			ASSERT(xmlTree.childs.at(0).value == "h1"_token);
			ASSERT(xmlTree.childs.at(0).childs.size() == 2);
			ASSERT(xmlTree.childs.at(0).childs.at(0).value == "attributes"_token);
			ASSERT(xmlTree.childs.at(0).childs.at(0).childs.size() == 1);
			ASSERT(xmlTree.childs.at(0).childs.at(0).childs.at(0).value == "color"_token);
			ASSERT(xmlTree.childs.at(0).childs.at(0).childs.at(0).childs.size() == 1);
			ASSERT(xmlTree.childs.at(0).childs.at(0).childs.at(0).childs.at(0).value == "red"s);
			ASSERT(xmlTree.childs.at(0).childs.at(1).value == "Title"s);
		});
	}
}
