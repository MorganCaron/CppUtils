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
			CppUtils::Log::TreeNodeLogger::log(xmlTree);

			TEST_ASSERT(xmlTree.exists("h1"_token));
			const auto& h1 = xmlTree.at("h1"_token);
			TEST_ASSERT(h1.getChildValue() == "Title"s);
		});

		addTest("Nested tags", [] {
			const auto xmlTree = R"(
			<ul>
				<li>First</li>
				<li>Second</li>
				<li>Third</li>
			</ul>
			)"_xml;
			CppUtils::Log::TreeNodeLogger::log(xmlTree);

			TEST_ASSERT(xmlTree.exists("ul"_token));
			const auto& ul = xmlTree.at("ul"_token);
			TEST_ASSERT(ul.childs.size() == 3);
			for (const auto& li : ul.childs)
				TEST_ASSERT(li.value == "li"_token);
			TEST_ASSERT(ul.childs.at(0).childs.size() == 1);
			TEST_ASSERT(ul.childs.at(0).getChildValue() == "First"s);
			TEST_ASSERT(ul.childs.at(1).getChildValue() == "Second"s);
			TEST_ASSERT(ul.childs.at(2).getChildValue() == "Third"s);
		});

		addTest("Attributes", [] {
			const auto xmlTree = R"(
			<h1 color="red">Title</h1>
			)"_xml;
			CppUtils::Log::TreeNodeLogger::log(xmlTree);

			TEST_ASSERT(xmlTree.exists("h1"_token));
			const auto& h1 = xmlTree.at("h1"_token);
			TEST_ASSERT(h1.at("attributes"_token).at("color"_token).getChildValue() == "red"s);
			TEST_ASSERT(h1.getChildValue(1) == "Title"s);
		});
	}
}
