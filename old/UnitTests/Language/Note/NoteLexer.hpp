#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Note::NoteLexer
{
	TEST_GROUP("Language/Note/NoteLexer")
	{
		using namespace std::literals;
		using namespace CppUtils::Type::Literals;
		using namespace CppUtils::Language::Note::Literals;

		addTest("Minimalist", [] {
			const auto noteTree = R"(
				Label:
					Text
			)"_note;
			CppUtils::Log::TreeNodeLogger::log(noteTree);

			EXPECT(noteTree.exists("Label"_token));
			EXPECT(noteTree.at("Label"_token).childs.size() == 1);
			EXPECT(noteTree.at("Label"_token).getChildValue() == "Text"s);
		});

		addTest("Multiple keys", [] {
			const auto noteTree = R"(
				Category:
					Title: Label
					Description:
						Text
			)"_note;
			CppUtils::Log::TreeNodeLogger::log(noteTree);

			EXPECT(noteTree.exists("Category"_token));
			EXPECT(noteTree.at("Category"_token).childs.size() == 2);
			EXPECT(noteTree.at("Category"_token).getChildValue(0) == "Title"_token);
			EXPECT(noteTree.at("Category"_token).at("Title"_token).getChildValue() == "Label"s);
			EXPECT(noteTree.at("Category"_token).getChildValue(1) == "Description"_token);
			EXPECT(noteTree.at("Category"_token).at("Description"_token).getChildValue() == "Text"s);
		});

		addTest("Multilines values", [] {
			const auto noteTree = R"(
				Category:
					Title: Label
					Description: `
						Lorem ipsum dolor sit amet,
						consectetur adipiscing elit,
						sed do eiusmod tempor incididunt
						ut labore et dolore magna aliqua.
					`
			)"_note;
			CppUtils::Log::TreeNodeLogger::log(noteTree);

			EXPECT(noteTree.exists("Category"_token));
			EXPECT(noteTree.at("Category"_token).childs.size() == 2);
			EXPECT(noteTree.at("Category"_token).getChildValue(0) == "Title"_token);
			EXPECT(noteTree.at("Category"_token).at("Title"_token).getChildValue() == "Label"s);
			EXPECT(noteTree.at("Category"_token).getChildValue(1) == "Description"_token);
			EXPECT(noteTree.at("Category"_token).at("Description"_token).getChildValue() == "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."s);
		});

		addTest("Multiple categories", [] {
			const auto noteTree = R"(
				Category1:
					Title: Label
					Description:
						Text
				Category2:
					Empty: _
					Content: Text
			)"_note;
			CppUtils::Log::TreeNodeLogger::log(noteTree);

			EXPECT(noteTree.childs.size() == 2);
			EXPECT(noteTree.exists("Category1"_token));
			EXPECT(noteTree.exists("Category2"_token));
			EXPECT(noteTree.at("Category2"_token).childs.size() == 2);
			EXPECT(noteTree.at("Category2"_token).getChildValue(0) == "Empty"_token);
			EXPECT(noteTree.at("Category2"_token).at("Empty"_token).childs.empty());
			EXPECT(noteTree.at("Category2"_token).getChildValue(1) == "Content"_token);
			EXPECT(noteTree.at("Category2"_token).at("Content"_token).getChildValue() == "Text"s);
		});
	}
}
