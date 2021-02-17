#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Lexer
{
	const auto tests = std::vector<CppUtils::Test>{

		CppUtils::Test{"Language/Lexer/Expression", [] {
			using namespace std::literals;
			using namespace CppUtils::Type::Literals;
			auto lexer = CppUtils::Language::Lexer::Lexer<CppUtils::Type::Token>{};
			auto& printExpression = lexer.newExpression("print"_token);
			auto& stringExpression = lexer.newExpression("string"_token);

			printExpression >> "print(" >> stringExpression >> ");";
			stringExpression >> "\"Hello World!\"";

			static constexpr auto src = "print(\"Hello World!\");"sv;
			const auto tokenTree = lexer.parseString("print"_token, src);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(std::get<CppUtils::Type::Token>(tokenTree.value) == "print"_token);
			ASSERT(tokenTree.childs.size() == 1);
			ASSERT(std::get<CppUtils::Type::Token>(tokenTree.childs.at(0).value) == "string"_token);
			ASSERT(tokenTree.childs.at(0).childs.size() == 0);
		}},

		CppUtils::Test{"Language/Lexer/Parsers", [] {
			using namespace std::literals;
			using namespace CppUtils::Type::Literals;
			auto lexer = CppUtils::Language::Lexer::Lexer<CppUtils::Type::Token>{};
			auto& printExpression = lexer.newExpression("print"_token);
			auto& stringExpression = lexer.newExpression("string"_token);

			printExpression
				>> CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token> >> "print("
				>> stringExpression
				>> CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token> >> ");";
			stringExpression
				>> CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token>
				>> CppUtils::Language::Parser::quoteParser<CppUtils::Type::Token>;

			static constexpr auto src = "print(\"Hello World!\");"sv;
			const auto tokenTree = lexer.parseString("print"_token, src);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(std::get<CppUtils::Type::Token>(tokenTree.value) == "print"_token);
			ASSERT(tokenTree.childs.size() == 1);
			ASSERT(std::get<CppUtils::Type::Token>(tokenTree.childs.at(0).value) == "string"_token);
			ASSERT(tokenTree.childs.at(0).childs.size() == 1);
			ASSERT(std::get<CppUtils::Type::Token>(tokenTree.childs.at(0).childs.at(0).value) == "Hello World!"_token);
			ASSERT(tokenTree.childs.at(0).childs.at(0).childs.size() == 0);
		}},

		CppUtils::Test{"Language/Lexer/Recurrence", [] {
			using namespace std::literals;
			using namespace CppUtils::Type::Literals;
			auto lexer = CppUtils::Language::Lexer::Lexer<CppUtils::Type::Token>{};
			auto& mainExpression = lexer.newExpression("main"_token);
			auto& printExpression = lexer.newExpression("print"_token);
			auto& stringExpression = lexer.newExpression("string"_token);

			mainExpression
				>> (printExpression >= 0)
				>> CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token>;
			printExpression
				>> CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token> >> "print("
				>> stringExpression
				>> CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token> >> ");";
			stringExpression
				>> CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token>
				>> CppUtils::Language::Parser::quoteParser<CppUtils::Type::Token>;

			static constexpr auto src = R"(
				print("Hello World!");
				print("Test");
				print("Ok");
			)"sv;
			const auto tokenTree = lexer.parseString("main"_token, src);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(std::get<CppUtils::Type::Token>(tokenTree.value) == "main"_token);
			ASSERT(tokenTree.childs.size() == 3);
			for (const auto& child : tokenTree.childs)
				ASSERT(std::get<CppUtils::Type::Token>(child.value) == "print"_token);
		}},

		CppUtils::Test{"Language/Lexer/Alternative", [] {
			using namespace std::literals;
			using namespace CppUtils::Type::Literals;
			auto lexer = CppUtils::Language::Lexer::Lexer<CppUtils::Type::Token>{};
			auto& mainExpression = lexer.newExpression("main"_token);
			auto& valueExpression = lexer.newExpression("value"_token);
			auto& keywordExpression = lexer.newExpression("keyword"_token);
			auto& stringExpression = lexer.newExpression("string"_token);

			mainExpression
				>> (valueExpression >= 0)
				>> CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token>;
			valueExpression
				>> CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token>
				>> (keywordExpression || stringExpression);
			keywordExpression
				>> CppUtils::Language::Parser::keywordParser<CppUtils::Type::Token>;
			stringExpression
				>> CppUtils::Language::Parser::quoteParser<CppUtils::Type::Token>;

			static constexpr auto src = R"(
				test "test" test "test"
			)"sv;
			const auto tokenTree = lexer.parseString("main"_token, src);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(std::get<CppUtils::Type::Token>(tokenTree.value) == "main"_token);
			ASSERT(tokenTree.childs.size() == 4);
			for (const auto& child : tokenTree.childs)
				ASSERT(std::get<CppUtils::Type::Token>(child.value) == "value"_token);
		}}

	};
}
