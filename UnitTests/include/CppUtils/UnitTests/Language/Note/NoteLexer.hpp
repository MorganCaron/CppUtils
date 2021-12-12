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

			ASSERT(noteTree.exists("Label"_token));
			ASSERT(noteTree.at("Label"_token).getChildValue() == "Text"s);
		});

		addTest("Multiple keys", [] {
			const auto noteTree = R"(
				Category:
					Title: Label
					Description:
						Text
			)"_note;
			CppUtils::Log::TreeNodeLogger::log(noteTree);

			ASSERT(noteTree.exists("Category"_token));
			ASSERT(noteTree.at("Category"_token).getChildValue(0) == "Title"_token);
			ASSERT(noteTree.at("Category"_token).at("Title"_token).getChildValue() == "Label"s);
			ASSERT(noteTree.at("Category"_token).getChildValue(1) == "Description"_token);
			ASSERT(noteTree.at("Category"_token).at("Description"_token).getChildValue() == "Text"s);
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

			ASSERT(noteTree.exists("Category"_token));
			ASSERT(noteTree.at("Category"_token).getChildValue(0) == "Title"_token);
			ASSERT(noteTree.at("Category"_token).at("Title"_token).getChildValue() == "Label"s);
			ASSERT(noteTree.at("Category"_token).getChildValue(1) == "Description"_token);
			ASSERT(noteTree.at("Category"_token).at("Description"_token).getChildValue() == "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."s);
		});
	}
}
