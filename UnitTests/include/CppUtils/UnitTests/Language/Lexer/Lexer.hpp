#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Lexer
{
	const auto tests = std::vector<CppUtils::Test>{

		CppUtils::Test{"Language/Lexer/Lexemes", [] {
			using namespace std::literals;
			using namespace CppUtils::Type::Literals;

			auto lexer = CppUtils::Language::Lexer::Lexer{};

			auto& printExpression = lexer.expression("print"_token);
			auto& stringExpression = lexer.expression("string"_token);

			printExpression >> "print(" >> stringExpression >> ");";
			stringExpression >> "\"Hello World!\"";

			static constexpr auto src = "print(\"Hello World!\");"sv;
			const auto tokenTree = lexer.parse("print"_token, src);

			CppUtils::Terminal::setConsoleOutputUTF8();
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.self == "print"_token);
			ASSERT(tokenTree.childs.size() == 1);
			ASSERT(tokenTree.childs.at(0).self == "string"_token);
			ASSERT(tokenTree.childs.at(0).childs.size() == 0);
		}},

		CppUtils::Test{"Language/Lexer/Parsers", [] {
			using namespace std::literals;
			using namespace CppUtils::Type::Literals;

			auto lexer = CppUtils::Language::Lexer::Lexer{};
			auto& printExpression = lexer.expression("print"_token);
			auto& stringExpression = lexer.expression("string"_token);

			printExpression
				>> CppUtils::Language::Parser::spaceParser >> "print("
				>> stringExpression
				>> CppUtils::Language::Parser::spaceParser >> ");";
			stringExpression
				>> CppUtils::Language::Parser::spaceParser
				>> CppUtils::Language::Parser::quoteParser;

			static constexpr auto src = "print(\"Hello World!\");"sv;
			const auto tokenTree = lexer.parse("print"_token, src);

			CppUtils::Terminal::setConsoleOutputUTF8();
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.self == "print"_token);
			ASSERT(tokenTree.childs.size() == 1);
			ASSERT(tokenTree.childs.at(0).self == "string"_token);
			ASSERT(tokenTree.childs.at(0).childs.size() == 1);
			ASSERT(tokenTree.childs.at(0).childs.at(0).self == "Hello World!"_token);
			ASSERT(tokenTree.childs.at(0).childs.at(0).childs.size() == 0);
		}},

		CppUtils::Test{"Language/Lexer/Recurrence", [] {
			using namespace std::literals;
			using namespace CppUtils::Type::Literals;

			auto lexer = CppUtils::Language::Lexer::Lexer{};
			auto& mainExpression = lexer.expression("main"_token);
			auto& printExpression = lexer.expression("print"_token);
			auto& stringExpression = lexer.expression("string"_token);

			mainExpression
				>> (printExpression >= 0)
				>> CppUtils::Language::Parser::spaceParser;
			printExpression
				>> CppUtils::Language::Parser::spaceParser >> "print("
				>> stringExpression
				>> CppUtils::Language::Parser::spaceParser >> ");";
			stringExpression
				>> CppUtils::Language::Parser::spaceParser
				>> CppUtils::Language::Parser::quoteParser;

			static constexpr auto src = R"(
				print("Hello World!");
				print("Test");
				print("Ok");
			)"sv;
			const auto tokenTree = lexer.parse("main"_token, src);

			CppUtils::Terminal::setConsoleOutputUTF8();
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.self == "main"_token);
			ASSERT(tokenTree.childs.size() == 3);
			for (const auto& child : tokenTree.childs)
				ASSERT(child.self == "print"_token);
		}},

		CppUtils::Test{"Language/Lexer/Contingence", [] {
			using namespace std::literals;
			using namespace CppUtils::Type::Literals;

			auto lexer = CppUtils::Language::Lexer::Lexer{};
			auto& mainExpression = lexer.expression("main"_token);
			auto& valueExpression = lexer.expression("value"_token);
			auto& keywordExpression = lexer.expression("keyword"_token);
			auto& stringExpression = lexer.expression("string"_token);

			mainExpression
				>> (valueExpression >= 0)
				>> CppUtils::Language::Parser::spaceParser;
			valueExpression
				>> CppUtils::Language::Parser::spaceParser
				>> (keywordExpression || stringExpression);
			keywordExpression
				>> CppUtils::Language::Parser::keywordParser;
			stringExpression
				>> CppUtils::Language::Parser::quoteParser;

			static constexpr auto src = R"(
				test "test" test "test"
			)"sv;
			const auto tokenTree = lexer.parse("main"_token, src);

			CppUtils::Terminal::setConsoleOutputUTF8();
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.self == "main"_token);
			ASSERT(tokenTree.childs.size() == 4);
			for (const auto& child : tokenTree.childs)
				ASSERT(child.self == "value"_token);
		}}

	};
}
