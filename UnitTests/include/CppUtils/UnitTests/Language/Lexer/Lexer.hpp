#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Lexer
{
	TEST_GROUP("Language/Lexer")
	{
		using namespace std::literals;
		using namespace CppUtils::Type::Literals;
		
		addTest("Expression", [] {
			auto lexer = CppUtils::Language::Lexer::Lexer<CppUtils::Type::Token>{};
			auto& printExpression = lexer.newExpression("print"_token);
			auto& stringExpression = lexer.newExpression("string"_token);

			printExpression >> "print(" >> stringExpression >> ");";
			stringExpression >> "\"Hello World!\"";

			static constexpr auto src = "print(\"Hello World!\");"sv;
			const auto tokenTree = lexer.parseString("print"_token, src);
			CppUtils::Log::TreeNodeLogger::log(tokenTree);

			ASSERT(tokenTree.value == "print"_token);
			ASSERT(tokenTree.childs.size() == 1);
			ASSERT(tokenTree.getChildValue() == "string"_token);
			ASSERT(tokenTree.at("string"_token).childs.size() == 0);
		});

		addTest("Parsers", [] {
			auto lexer = CppUtils::Language::Lexer::Lexer<CppUtils::Type::Token, std::string>{};
			auto& printExpression = lexer.newExpression("print"_token);
			auto& stringExpression = lexer.newExpression("string"_token);

			printExpression
				>> CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token, std::string> >> "print("
				>> stringExpression
				>> CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token, std::string> >> ");";
			stringExpression
				>> CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token, std::string>
				>> CppUtils::Language::Parser::quoteParser<CppUtils::Type::Token, std::string>;

			static constexpr auto src = "print(\"Hello World!\");"sv;
			const auto tokenTree = lexer.parseString("print"_token, src);
			CppUtils::Log::TreeNodeLogger::log(tokenTree);

			ASSERT(tokenTree.value == "print"_token);
			ASSERT(tokenTree.childs.size() == 1);
			ASSERT(tokenTree.getChildValue() == "string"_token);
			ASSERT(tokenTree.at("string"_token).childs.size() == 1);
			ASSERT(tokenTree.at("string"_token).getChildValue() == "Hello World!"s);
			ASSERT(tokenTree.at("string"_token).at("Hello World!"s).childs.size() == 0);
		});

		addTest("Recurrence", [] {
			auto lexer = CppUtils::Language::Lexer::Lexer<CppUtils::Type::Token, std::string>{};
			auto& mainExpression = lexer.newExpression("main"_token);
			auto& printExpression = lexer.newExpression("print"_token);
			auto& stringExpression = lexer.newExpression("string"_token);

			mainExpression
				>> (printExpression >= 0)
				>> CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token, std::string>;
			printExpression
				>> CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token, std::string> >> "print("
				>> stringExpression
				>> CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token, std::string> >> ");";
			stringExpression
				>> CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token, std::string>
				>> CppUtils::Language::Parser::quoteParser<CppUtils::Type::Token, std::string>;

			static constexpr auto src = R"(
				print("Hello World!");
				print("Test");
				print("Ok");
			)"sv;
			const auto tokenTree = lexer.parseString("main"_token, src);
			CppUtils::Log::TreeNodeLogger::log(tokenTree);

			ASSERT(tokenTree.value == "main"_token);
			ASSERT(tokenTree.childs.size() == 3);
			for (const auto& child : tokenTree.childs)
				ASSERT(child.value == "print"_token);
		});

		addTest("Alternative", [] {
			auto lexer = CppUtils::Language::Lexer::Lexer<CppUtils::Type::Token, std::string>{};
			auto& mainExpression = lexer.newExpression("main"_token);
			auto& valueExpression = lexer.newExpression("value"_token);
			auto& keywordExpression = lexer.newExpression("keyword"_token);
			auto& stringExpression = lexer.newExpression("string"_token);

			mainExpression
				>> (valueExpression >= 0)
				>> CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token, std::string>;
			valueExpression
				>> CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token, std::string>
				>> (keywordExpression || stringExpression);
			keywordExpression
				>> CppUtils::Language::Parser::keywordParser<CppUtils::Type::Token, std::string>;
			stringExpression
				>> CppUtils::Language::Parser::quoteParser<CppUtils::Type::Token, std::string>;

			static constexpr auto src = R"(
				test "test" test "test"
			)"sv;
			const auto tokenTree = lexer.parseString("main"_token, src);
			CppUtils::Log::TreeNodeLogger::log(tokenTree);

			ASSERT(tokenTree.value == "main"_token);
			ASSERT(tokenTree.childs.size() == 4);
			for (const auto& child : tokenTree.childs)
				ASSERT(child.value == "value"_token);
		});

		addTest("Exclusion", [] {
			auto lexer = CppUtils::Language::Lexer::Lexer<CppUtils::Type::Token>{};
			auto& mainExpression = lexer.newExpression("main"_token);
			auto& aExpression = lexer.newExpression("a"_token);
			auto& tagsExpression = lexer.newExpression("tags"_token, false);

			mainExpression >> tagsExpression >> 'b';
			aExpression >> 'a';
			tagsExpression >> CppUtils::Language::Parser::Recurrence{
					std::make_unique<CppUtils::Language::Parser::ExcludeLexeme>(CppUtils::Language::Parser::Exclusion{
						std::make_unique<CppUtils::Language::Parser::TokenLexeme>(aExpression.token),
						std::make_unique<CppUtils::Language::Parser::StringLexeme>("b")}),
				CppUtils::Language::Parser::RecurrenceType::MoreOrEqualTo, 0};

			static constexpr auto src = "aab"sv;
			const auto tokenTree = lexer.parseString("main"_token, src);
			CppUtils::Log::TreeNodeLogger::log(tokenTree);

			ASSERT(tokenTree.value == "main"_token);
			ASSERT(tokenTree.childs.size() == 2);
			for (const auto& child : tokenTree.childs)
				ASSERT(child.value == "a"_token);
		});
	}
}
