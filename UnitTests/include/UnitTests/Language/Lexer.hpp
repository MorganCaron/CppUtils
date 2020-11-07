#pragma once

#include <CppUtils.hpp>

namespace UnitTests::Language::Lexer
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("Language/Lexer/Lexemes", [] {
			using namespace CppUtils::Type::Literals;

			auto lexer = CppUtils::Language::Lexer{};

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
		}),

		CppUtils::Test::UnitTest("Language/Lexer/Parsers", [] {
			using namespace CppUtils::Type::Literals;

			auto lexer = CppUtils::Language::Lexer{};
			auto& printLexeme = lexer.lexeme("print"_typeId);
			auto& stringLexeme = lexer.lexeme("string"_typeId);

			printLexeme
				>> CppUtils::Language::Parser::spaceParser >> "print("
				>> stringLexeme
				>> CppUtils::Language::Parser::spaceParser >> ");";
			stringLexeme
				>> CppUtils::Language::Parser::spaceParser
				>> CppUtils::Language::Parser::quoteParser;

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
		}),

		CppUtils::Test::UnitTest("Language/Lexer/Recurrence", [] {
			using namespace CppUtils::Type::Literals;

			auto lexer = CppUtils::Language::Lexer{};
			auto& mainLexeme = lexer.lexeme("main"_typeId);
			auto& printLexeme = lexer.lexeme("print"_typeId);
			auto& stringLexeme = lexer.lexeme("string"_typeId);

			mainLexeme
				>> (printLexeme >= 0)
				>> CppUtils::Language::Parser::spaceParser;
			printLexeme
				>> CppUtils::Language::Parser::spaceParser >> "print("
				>> stringLexeme
				>> CppUtils::Language::Parser::spaceParser >> ");";
			stringLexeme
				>> CppUtils::Language::Parser::spaceParser
				>> CppUtils::Language::Parser::quoteParser;

			static constexpr auto src = "\
			print(\"Hello World!\");\
			print(\"Test\");\
			print(\"Ok\");"sv;
			const auto tokenTree = lexer.parse("main"_typeId, src);

			CppUtils::Terminal::setConsoleOutputUTF8();
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.self == "main"_typeId);
			ASSERT(tokenTree.childs.size() == 3);
			for (const auto& child : tokenTree.childs)
				ASSERT(child.self == "print"_typeId);
		}),

		CppUtils::Test::UnitTest("Language/Lexer/Contingence", [] {
			using namespace CppUtils::Type::Literals;

			auto lexer = CppUtils::Language::Lexer{};
			auto& mainLexeme = lexer.lexeme("main"_typeId);
			auto& valueLexeme = lexer.lexeme("value"_typeId);
			auto& keywordLexeme = lexer.lexeme("keyword"_typeId);
			auto& stringLexeme = lexer.lexeme("string"_typeId);

			mainLexeme
				>> (valueLexeme >= 0)
				>> CppUtils::Language::Parser::spaceParser;
			valueLexeme
				>> CppUtils::Language::Parser::spaceParser
				>> (keywordLexeme || stringLexeme);
			keywordLexeme
				>> CppUtils::Language::Parser::keywordParser;
			stringLexeme
				>> CppUtils::Language::Parser::quoteParser;

			static constexpr auto src = "\
			test \"test\" test \"test\"\
			"sv;
			const auto tokenTree = lexer.parse("main"_typeId, src);

			CppUtils::Terminal::setConsoleOutputUTF8();
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.self == "main"_typeId);
			ASSERT(tokenTree.childs.size() == 4);
			for (const auto& child : tokenTree.childs)
				ASSERT(child.self == "value"_typeId);
		})

	};
}
