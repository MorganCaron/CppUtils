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

			ASSERT(xmlTree.exists("h1"_token));
			const auto& h1 = xmlTree.at("h1"_token);
			ASSERT(h1.getChildValue(0) == "attributes"_token);
			ASSERT(h1.getChildValue(1) == "Title"s);
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

			ASSERT(xmlTree.exists("ul"_token));
			const auto& ul = xmlTree.at("ul"_token);
			ASSERT(ul.childs.size() == 4);
			ASSERT(ul.getChildValue(0) == "attributes"_token);
			for (auto i = 1u; i <= 3; ++i)
				ASSERT(ul.getChildValue(i) == "li"_token);
			ASSERT(ul.childs.at(1).childs.size() == 2);
			ASSERT(ul.childs.at(1).getChildValue(0) == "attributes"_token);
			ASSERT(ul.childs.at(1).getChildValue(1) == "First"s);
		});

		addTest("Attributes", [] {
			const auto xmlTree = R"(
			<h1 color="red">Title</h1>
			)"_xml;
			CppUtils::Graph::logTreeNode(xmlTree);

			ASSERT(xmlTree.exists("h1"_token));
			const auto& h1 = xmlTree.at("h1"_token);
			ASSERT(h1.at("attributes"_token).at("color"_token).getChildValue() == "red"s);
			ASSERT(h1.getChildValue(1) == "Title"s);
		});
	}
}
