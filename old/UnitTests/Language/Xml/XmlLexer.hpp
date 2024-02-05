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

			EXPECT(xmlTree.exists("h1"_token));
			const auto& h1 = xmlTree.at("h1"_token);
			EXPECT(h1.getChildValue() == "Title"s);
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

			EXPECT(xmlTree.exists("ul"_token));
			const auto& ul = xmlTree.at("ul"_token);
			EXPECT(ul.childs.size() == 3);
			for (const auto& li : ul.childs)
				EXPECT(li.value == "li"_token);
			EXPECT(ul.childs.at(0).childs.size() == 1);
			EXPECT(ul.childs.at(0).getChildValue() == "First"s);
			EXPECT(ul.childs.at(1).getChildValue() == "Second"s);
			EXPECT(ul.childs.at(2).getChildValue() == "Third"s);
		});

		addTest("Attributes", [] {
			const auto xmlTree = R"(
			<h1 color="red">Title</h1>
			)"_xml;
			CppUtils::Log::TreeNodeLogger::log(xmlTree);

			EXPECT(xmlTree.exists("h1"_token));
			const auto& h1 = xmlTree.at("h1"_token);
			EXPECT(h1.at("attributes"_token).at("color"_token).getChildValue() == "red"s);
			EXPECT(h1.getChildValue(1) == "Title"s);
		});
	}
}
