#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Parser::Cursor
{
	const auto tests = std::vector<CppUtils::Test>{

		CppUtils::Test{"Language/Parser/Cursor/isEndOfString", [] {
			using namespace std::literals;

			const auto src = "A"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor{src, pos};

			ASSERT(cursor.isEndOfString() == false);
			++cursor.pos;
			ASSERT(cursor.isEndOfString() == true);
		}},

		CppUtils::Test{"Language/Parser/Cursor/getChar", [] {
			using namespace std::literals;

			const auto src = "AZ"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor{src, pos};

			ASSERT(cursor.getChar() == 'A');
			++cursor.pos;
			ASSERT(cursor.getChar() == 'Z');
		}},

		CppUtils::Test{"Language/Parser/Cursor/getCharAndSkipIt", [] {
			using namespace std::literals;

			const auto src = "AZ"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor{src, pos};

			ASSERT(cursor.getCharAndSkipIt() == 'A');
			ASSERT(cursor.getCharAndSkipIt() == 'Z');
		}},

		CppUtils::Test{"Language/Parser/Cursor/skipSpaces", [] {
			using namespace std::literals;

			const auto src = "\n 	 \n	 	A"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor{src, pos};

			cursor.skipSpaces();
			ASSERT(cursor.isEndOfString() == false);
			ASSERT(cursor.getChar() == 'A');
		}},

		CppUtils::Test{"Language/Parser/Cursor/getNextNChar", [] {
			using namespace std::literals;
			
			const auto src = "Hello world!"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor{src, pos};

			auto word = cursor.getNextNChar(11);
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "Hello world");

			word = cursor.getNextNChar(5);
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "Hello");
		}},

		CppUtils::Test{"Language/Parser/Cursor/getWord", [] {
			using namespace std::literals;

			const auto src = "Hello world!"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor{src, pos};

			auto word = cursor.getWord();
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "Hello");

			word = cursor.getWord();
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "Hello");
		}},

		CppUtils::Test{"Language/Parser/Cursor/getWordAndSkipIt", [] {
			using namespace std::literals;

			const auto src = "Hello world!"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor{src, pos};

			auto word = cursor.getWordAndSkipIt();
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "Hello");

			cursor.skipSpaces();

			word = cursor.getWordAndSkipIt();
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "world");
		}},

		CppUtils::Test{"Language/Parser/Cursor/getKeyword", [] {
			using namespace std::literals;

			const auto src = "variable_name0 functionName(argument);"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor{src, pos};

			auto keyword = cursor.getKeyword();
			CppUtils::Log::Logger::logInformation(keyword);
			ASSERT(keyword == "variable_name0");

			keyword = cursor.getKeyword();
			CppUtils::Log::Logger::logInformation(keyword);
			ASSERT(keyword == "variable_name0");
		}},

		CppUtils::Test{"Language/Parser/Cursor/getKeywordAndSkipIt", [] {
			using namespace std::literals;

			const auto src = "variable_name0 functionName(argument);"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor{src, pos};

			auto word = cursor.getKeywordAndSkipIt();
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "variable_name0");

			cursor.skipSpaces();

			word = cursor.getKeywordAndSkipIt();
			CppUtils::Log::Logger::logInformation(word);
			ASSERT(word == "functionName");
		}},

		CppUtils::Test{"Language/Parser/Cursor/getKeywordRequired", [] {
			using namespace std::literals;
			
			const auto src = "variable_name0 functionName(argument);"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor{src, pos};

			const auto keyword = cursor.getKeywordRequired("error message");
			CppUtils::Log::Logger::logInformation(keyword);
			ASSERT(keyword == "variable_name0");
		}},

		CppUtils::Test{"Language/Parser/Cursor/isEqualSkipIt", [] {
			using namespace std::literals;
			
			const auto src = "Hello world!"sv;
			auto pos = std::size_t{0};
			auto cursor = CppUtils::Language::Parser::Cursor{src, pos};

			ASSERT(cursor.isEqualSkipIt("Hello") == true);
			cursor.skipSpaces();
			ASSERT(cursor.isEqualSkipIt("Hello") == false);
		}}

	};
}
