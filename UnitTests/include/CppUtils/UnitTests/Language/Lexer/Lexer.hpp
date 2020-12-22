#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Lexer
{
	const auto tests = std::vector<CppUtils::Test>{

		CppUtils::Test{"Language/Lexer/Lexemes", [] {
			using namespace std::literals;
			using namespace CppUtils::Type::Literals;

			auto lexer = CppUtils::Language::Lexer::Lexer{};

			auto& printLexeme = lexer.lexeme("print"_typeId);
			auto& stringLexeme = lexer.lexeme("string"_typeId);

			printLexeme >> "print(" >> stringLexeme >> ");";
			stringLexeme >> "\"Hello World!\"";

			static constexpr auto src = "print(\"Hello World!\");"sv;
			const auto tokenTree = lexer.parse("print"_typeId, src);

			CppUtils::Terminal::setConsoleOutputUTF8();
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.self == "print"_typeId);
			ASSERT(tokenTree.childs.size() == 1);
			ASSERT(tokenTree.childs.at(0).self == "string"_typeId);
			ASSERT(tokenTree.childs.at(0).childs.size() == 0);
		}},

		CppUtils::Test{"Language/Lexer/Parsers", [] {
			using namespace std::literals;
			using namespace CppUtils::Type::Literals;

			auto lexer = CppUtils::Language::Lexer::Lexer{};
			auto& printLexeme = lexer.lexeme("print"_typeId);
			auto& stringLexeme = lexer.lexeme("string"_typeId);

			printLexeme
				>> CppUtils::Language::Lexer::Parsers::spaceParser >> "print("
				>> stringLexeme
				>> CppUtils::Language::Lexer::Parsers::spaceParser >> ");";
			stringLexeme
				>> CppUtils::Language::Lexer::Parsers::spaceParser
				>> CppUtils::Language::Lexer::Parsers::quoteParser;

			static constexpr auto src = "print(\"Hello World!\");"sv;
			const auto tokenTree = lexer.parse("print"_typeId, src);

			CppUtils::Terminal::setConsoleOutputUTF8();
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.self == "print"_typeId);
			ASSERT(tokenTree.childs.size() == 1);
			ASSERT(tokenTree.childs.at(0).self == "string"_typeId);
			ASSERT(tokenTree.childs.at(0).childs.size() == 1);
			ASSERT(tokenTree.childs.at(0).childs.at(0).self == "Hello World!"_typeId);
			ASSERT(tokenTree.childs.at(0).childs.at(0).childs.size() == 0);
		}},

		CppUtils::Test{"Language/Lexer/Recurrence", [] {
			using namespace std::literals;
			using namespace CppUtils::Type::Literals;

			auto lexer = CppUtils::Language::Lexer::Lexer{};
			auto& mainLexeme = lexer.lexeme("main"_typeId);
			auto& printLexeme = lexer.lexeme("print"_typeId);
			auto& stringLexeme = lexer.lexeme("string"_typeId);

			mainLexeme
				>> (printLexeme >= 0)
				>> CppUtils::Language::Lexer::Parsers::spaceParser;
			printLexeme
				>> CppUtils::Language::Lexer::Parsers::spaceParser >> "print("
				>> stringLexeme
				>> CppUtils::Language::Lexer::Parsers::spaceParser >> ");";
			stringLexeme
				>> CppUtils::Language::Lexer::Parsers::spaceParser
				>> CppUtils::Language::Lexer::Parsers::quoteParser;

			static constexpr auto src = R"(
				print("Hello World!");
				print("Test");
				print("Ok");
			)"sv;
			const auto tokenTree = lexer.parse("main"_typeId, src);

			CppUtils::Terminal::setConsoleOutputUTF8();
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.self == "main"_typeId);
			ASSERT(tokenTree.childs.size() == 3);
			for (const auto& child : tokenTree.childs)
				ASSERT(child.self == "print"_typeId);
		}},

		CppUtils::Test{"Language/Lexer/Contingence", [] {
			using namespace std::literals;
			using namespace CppUtils::Type::Literals;

			auto lexer = CppUtils::Language::Lexer::Lexer{};
			auto& mainLexeme = lexer.lexeme("main"_typeId);
			auto& valueLexeme = lexer.lexeme("value"_typeId);
			auto& keywordLexeme = lexer.lexeme("keyword"_typeId);
			auto& stringLexeme = lexer.lexeme("string"_typeId);

			mainLexeme
				>> (valueLexeme >= 0)
				>> CppUtils::Language::Lexer::Parsers::spaceParser;
			valueLexeme
				>> CppUtils::Language::Lexer::Parsers::spaceParser
				>> (keywordLexeme || stringLexeme);
			keywordLexeme
				>> CppUtils::Language::Lexer::Parsers::keywordParser;
			stringLexeme
				>> CppUtils::Language::Lexer::Parsers::quoteParser;

			static constexpr auto src = R"(
				test "test" test "test"
			)"sv;
			const auto tokenTree = lexer.parse("main"_typeId, src);

			CppUtils::Terminal::setConsoleOutputUTF8();
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.self == "main"_typeId);
			ASSERT(tokenTree.childs.size() == 4);
			for (const auto& child : tokenTree.childs)
				ASSERT(child.self == "value"_typeId);
		}}

	};
}
