#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Parser
{
	using namespace std::literals;
	
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("Language/Parser/Cursor/isEndOfString", [] {
			const auto src = "A"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Reader::Cursor{src, pos};

			ASSERT(cursor.isEndOfString() == false);
			++cursor.pos;
			ASSERT(cursor.isEndOfString() == true);
		}),

		CppUtils::Test::UnitTest("Language/Parser/Cursor/getChar", [] {
			const auto src = "AZ"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Reader::Cursor{src, pos};

			ASSERT(cursor.getChar() == 'A');
			++cursor.pos;
			ASSERT(cursor.getChar() == 'Z');
		}),

		CppUtils::Test::UnitTest("Language/Parser/Cursor/getCharAndSkipIt", [] {
			const auto src = "AZ"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Reader::Cursor{src, pos};

			ASSERT(cursor.getCharAndSkipIt() == 'A');
			ASSERT(cursor.getCharAndSkipIt() == 'Z');
		}),

		CppUtils::Test::UnitTest("Language/Parser/Cursor/skipSpaces", [] {
			const auto src = "\n 	 \n	 	A"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Reader::Cursor{src, pos};

			cursor.skipSpaces();
			ASSERT(cursor.isEndOfString() == false);
			ASSERT(cursor.getChar() == 'A');
		}),

		CppUtils::Test::UnitTest("Language/Parser/Cursor/getNextNChar", [] {
			const auto src = "Hello world!"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Reader::Cursor{src, pos};

			auto word = cursor.getNextNChar(11);
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "Hello world");

			word = cursor.getNextNChar(5);
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "Hello");
		}),

		CppUtils::Test::UnitTest("Language/Parser/Cursor/getWord", [] {
			const auto src = "Hello world!"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Reader::Cursor{src, pos};

			auto word = cursor.getWord();
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "Hello");

			word = cursor.getWord();
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "Hello");
		}),

		CppUtils::Test::UnitTest("Language/Parser/Cursor/getWordAndSkipIt", [] {
			const auto src = "Hello world!"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Reader::Cursor{src, pos};

			auto word = cursor.getWordAndSkipIt();
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "Hello");

			cursor.skipSpaces();

			word = cursor.getWordAndSkipIt();
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "world");
		}),

		CppUtils::Test::UnitTest("Language/Parser/Cursor/getKeyword", [] {
			const auto src = "variable_name0 functionName(argument);"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Reader::Cursor{src, pos};

			auto keyword = cursor.getKeyword();
			CppUtils::Log::Logger::logInformation(keyword);
			ASSERT(keyword == "variable_name0");

			keyword = cursor.getKeyword();
			CppUtils::Log::Logger::logInformation(keyword);
			ASSERT(keyword == "variable_name0");
		}),

		CppUtils::Test::UnitTest("Language/Parser/Cursor/getKeywordAndSkipIt", [] {
			const auto src = "variable_name0 functionName(argument);"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Reader::Cursor{src, pos};

			auto word = cursor.getKeywordAndSkipIt();
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "variable_name0");

			cursor.skipSpaces();

			word = cursor.getKeywordAndSkipIt();
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "functionName");
		}),

		CppUtils::Test::UnitTest("Language/Parser/Cursor/getKeywordRequired", [] {
			const auto src = "variable_name0 functionName(argument);"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Reader::Cursor{src, pos};

			const auto keyword = cursor.getKeywordRequired("error message");
			CppUtils::Log::Logger::logInformation(keyword);
			ASSERT(keyword == "variable_name0");
		}),

		CppUtils::Test::UnitTest("Language/Parser/Cursor/isEqualSkipIt", [] {
			const auto src = "Hello world!"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Reader::Cursor{src, pos};

			ASSERT(cursor.isEqualSkipIt("Hello") == true);
			cursor.skipSpaces();
			ASSERT(cursor.isEqualSkipIt("Hello") == false);
		})

	};
}
