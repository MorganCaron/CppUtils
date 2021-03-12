#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Parser::Cursor
{
	TEST_GROUP("Language/Parser/Cursor")
	{
		using namespace std::literals;
		
		addTest("isEndOfString", [] {
			const auto src = "A"sv;
			auto position = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor<std::string>{src, position};

			ASSERT(cursor.isEndOfString() == false);
			++cursor.position;
			ASSERT(cursor.isEndOfString() == true);
		});

		addTest("getChar", [] {
			const auto src = "AZ"sv;
			auto position = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor<std::string>{src, position};

			ASSERT(cursor.getChar() == 'A');
			++cursor.position;
			ASSERT(cursor.getChar() == 'Z');
		});

		addTest("getCharAndSkipIt", [] {
			const auto src = "AZ"sv;
			auto position = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor<std::string>{src, position};

			ASSERT(cursor.getCharAndSkipIt() == 'A');
			ASSERT(cursor.getCharAndSkipIt() == 'Z');
		});

		addTest("skipSpaces", [] {
			const auto src = "\n 	 \n	 	A"sv;
			auto position = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor<std::string>{src, position};

			cursor.skipSpaces();
			ASSERT(cursor.isEndOfString() == false);
			ASSERT(cursor.getChar() == 'A');
		});

		addTest("getNextNChar", [] {
			const auto src = "Hello world!"sv;
			auto position = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor<std::string>{src, position};

			auto word = cursor.getNextNChar(11);
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "Hello world");

			word = cursor.getNextNChar(5);
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "Hello");
		});

		addTest("getWord", [] {
			const auto src = "Hello world!"sv;
			auto position = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor<std::string>{src, position};

			auto word = cursor.getWord();
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "Hello");

			word = cursor.getWord();
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "Hello");
		});

		addTest("getWordAndSkipIt", [] {
			const auto src = "Hello world!"sv;
			auto position = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor<std::string>{src, position};

			auto word = cursor.getWordAndSkipIt();
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "Hello");

			cursor.skipSpaces();

			word = cursor.getWordAndSkipIt();
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "world");
		});

		addTest("getKeyword", [] {
			const auto src = "variable_name0 functionName(argument);"sv;
			auto position = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor<std::string>{src, position};

			auto keyword = cursor.getKeyword();
			CppUtils::Log::Logger::logInformation(keyword);
			ASSERT(keyword == "variable_name0");

			keyword = cursor.getKeyword();
			CppUtils::Log::Logger::logInformation(keyword);
			ASSERT(keyword == "variable_name0");
		});

		addTest("getKeywordAndSkipIt", [] {
			const auto src = "variable_name0 functionName(argument);"sv;
			auto position = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor<std::string>{src, position};

			auto word = cursor.getKeywordAndSkipIt();
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "variable_name0");

			cursor.skipSpaces();

			word = cursor.getKeywordAndSkipIt();
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "functionName");
		});

		addTest("getKeywordRequired", [] {
			const auto src = "variable_name0 functionName(argument);"sv;
			auto position = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor<std::string>{src, position};

			const auto keyword = cursor.getKeywordRequired("error message");
			CppUtils::Log::Logger::logInformation(keyword);
			ASSERT(keyword == "variable_name0");
		});

		addTest("isEqualSkipIt", [] {
			const auto src = "Hello world!"sv;
			auto position = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor<std::string>{src, position};

			ASSERT(cursor.isEqualSkipIt("Hello") == true);
			cursor.skipSpaces();
			ASSERT(cursor.isEqualSkipIt("Hello") == false);
		});
	}
}
