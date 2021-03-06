#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Lexer::GrammarLexer
{
	TEST_GROUP("Language/Lexer/GrammarLexer")
	{
		using namespace std::literals;
		using namespace CppUtils::Type::Literals;
		
		addTest("Lexeme", [] {
			auto grammarLexer = CppUtils::Language::Lexer::GrammarLexer<CppUtils::Type::Token>{};

			static constexpr auto grammarSrc = R"(
			main: "Test";
			)"sv;
			const auto grammarTree = grammarLexer.parseGrammar(grammarSrc);
			CppUtils::Graph::logTreeNode(grammarTree);
			
			static constexpr auto languageSrc = "Test"sv;
			const auto tokenTree = grammarLexer.parseLanguage("main"_token, languageSrc);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.value == "main"_token);
			ASSERT(tokenTree.childs.size() == 0);
		});

		addTest("Expressions", [] {
			auto grammarLexer = CppUtils::Language::Lexer::GrammarLexer<CppUtils::Type::Token, std::string>{};

			grammarLexer.addParsingFunction("spaceParser"_token, CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token, std::string>);
			grammarLexer.addParsingFunction("keywordParser"_token, CppUtils::Language::Parser::keywordParser<CppUtils::Type::Token, std::string>);
			grammarLexer.addParsingFunction("quoteParser"_token, CppUtils::Language::Parser::quoteParser<CppUtils::Type::Token, std::string>);
			
			static constexpr auto grammarSrc = R"(
			main: instruction spaceParser;
			!instruction: function spaceParser ';';
			function: spaceParser keywordParser spaceParser '(' argument spaceParser ')';
			!argument: string;
			string: spaceParser quoteParser;
			)"sv;
			const auto grammarTree = grammarLexer.parseGrammar(grammarSrc);
			CppUtils::Graph::logTreeNode(grammarTree);
			
			static constexpr auto languageSrc = R"(
				print("Hello World!");
			)"sv;
			const auto tokenTree = grammarLexer.parseLanguage("main"_token, languageSrc);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.value == "main"_token);
			ASSERT(tokenTree.childs.size() == 1);
			ASSERT(tokenTree.childs.at(0).value == "function"_token);
			ASSERT(tokenTree.childs.at(0).childs.size() == 2);
			ASSERT(tokenTree.childs.at(0).childs.at(0).value == "print"_token);
			ASSERT(tokenTree.childs.at(0).childs.at(1).value == "string"_token);
			ASSERT(tokenTree.childs.at(0).childs.at(1).childs.size() == 1);
			ASSERT(tokenTree.childs.at(0).childs.at(1).childs.at(0).value == "Hello World!"s);
		});

		addTest("Hide", [] {
			auto grammarLexer = CppUtils::Language::Lexer::GrammarLexer<CppUtils::Type::Token, std::string>{};

			grammarLexer.addParsingFunction("spaceParser"_token, CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token, std::string>);
			
			static constexpr auto grammarSrc = R"(
			main: word spaceParser;
			!word: spaceParser 'hide';
			)"sv;
			const auto grammarTree = grammarLexer.parseGrammar(grammarSrc);
			CppUtils::Graph::logTreeNode(grammarTree);
			
			static constexpr auto languageSrc = "hide"sv;
			const auto tokenTree = grammarLexer.parseLanguage("main"_token, languageSrc);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.value == "main"_token);
			ASSERT(tokenTree.childs.empty());
		});

		addTest("Name", [] {
			auto grammarLexer = CppUtils::Language::Lexer::GrammarLexer<CppUtils::Type::Token, std::string>{};

			grammarLexer.addParsingFunction("spaceParser"_token, CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token, std::string>);
			
			static constexpr auto grammarSrc = R"(
			main: word spaceParser;
			word[name]: spaceParser 'name';
			)"sv;
			const auto grammarTree = grammarLexer.parseGrammar(grammarSrc);
			CppUtils::Graph::logTreeNode(grammarTree);
			
			static constexpr auto languageSrc = "name"sv;
			const auto tokenTree = grammarLexer.parseLanguage("main"_token, languageSrc);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.value == "main"_token);
			ASSERT(tokenTree.getChildValue() == "name"_token);
		});

		addTest("Recurrence", [] {
			auto grammarLexer = CppUtils::Language::Lexer::GrammarLexer<CppUtils::Type::Token>{};

			grammarLexer.addParsingFunction("spaceParser"_token, CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token>);
			grammarLexer.addParsingFunction("keywordParser"_token, CppUtils::Language::Parser::keywordParser<CppUtils::Type::Token>);
			
			static constexpr auto grammarSrc = R"(
			main: (word >= 0) spaceParser;
			!word: spaceParser keywordParser;
			)"sv;
			const auto grammarTree = grammarLexer.parseGrammar(grammarSrc);
			CppUtils::Graph::logTreeNode(grammarTree);
			
			static constexpr auto languageSrc = "This is a test"sv;
			const auto tokenTree = grammarLexer.parseLanguage("main"_token, languageSrc);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.value == "main"_token);
			ASSERT(tokenTree.childs.size() == 4);
		});

		addTest("Exclusion", [] {
			auto grammarLexer = CppUtils::Language::Lexer::GrammarLexer<CppUtils::Type::Token, std::string>{};
			static constexpr auto grammarSrc = R"(
			main: tags 'b';
			!tags: ((a != 'b') >= 0);
			a: 'a';
			)"sv;
			const auto grammarTree = grammarLexer.parseGrammar(grammarSrc);
			CppUtils::Graph::logTreeNode(grammarTree);
			
			static constexpr auto languageSrc = "aab"sv;
			const auto tokenTree = grammarLexer.parseLanguage("main"_token, languageSrc);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.value == "main"_token);
			ASSERT(tokenTree.childs.size() == 2);
			for (const auto& child : tokenTree.childs)
				ASSERT(child.value == "a"_token);
		});

		addTest("Avanced", [] {
			auto grammarLexer = CppUtils::Language::Lexer::GrammarLexer<CppUtils::Type::Token, std::string>{};

			grammarLexer.addParsingFunction("spaceParser"_token, CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token, std::string>);
			grammarLexer.addParsingFunction("keywordParser"_token, CppUtils::Language::Parser::keywordParser<CppUtils::Type::Token, std::string>);
			grammarLexer.addParsingFunction("quoteParser"_token, CppUtils::Language::Parser::quoteParser<CppUtils::Type::Token, std::string>);
			
			static constexpr auto grammarSrc = R"(
			main: (instruction >= 0) spaceParser;
			!instruction: (function || variableDeclaration) spaceParser ';';
			function: spaceParser keywordParser spaceParser '(' ~arguments spaceParser ')';
			!arguments: value ~secondaryArgument;
			!secondaryArgument: spaceParser ',' arguments;
			variableDeclaration: spaceParser keywordParser spaceParser '=' string;
			!value: (string || variableCall);
			string: spaceParser quoteParser;
			variableCall: spaceParser keywordParser;
			)"sv;
			const auto grammarTree = grammarLexer.parseGrammar(grammarSrc);
			CppUtils::Graph::logTreeNode(grammarTree);
			
			static constexpr auto languageSrc = R"(
			print("Hello World!");
			variable = "azerty";
			print("Test", variable, "123");
			print("Ok");
			)"sv;
			const auto tokenTree = grammarLexer.parseLanguage("main"_token, languageSrc);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.value == "main"_token);
			ASSERT(tokenTree.childs.size() == 4);
		});
	}
}
