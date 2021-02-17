#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Lexer::GrammarLexer
{
	using namespace std::literals;
	
	const auto tests = std::vector<CppUtils::Test>{

		CppUtils::Test{"Language/Lexer/GrammarLexer/Lexeme", [] {
			using namespace CppUtils::Type::Literals;
			auto grammarLexer = CppUtils::Language::Lexer::GrammarLexer<CppUtils::Type::Token>{};

			static constexpr auto grammarSrc = R"(
			main: "Test";
			)"sv;
			const auto grammarTree = grammarLexer.parseGrammar(grammarSrc);
			CppUtils::Graph::logTreeNode(grammarTree);
			
			static constexpr auto languageSrc = "Test"sv;
			const auto tokenTree = grammarLexer.parseLanguage("main"_token, languageSrc);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(std::get<CppUtils::Type::Token>(tokenTree.value) == "main"_token);
			ASSERT(tokenTree.childs.size() == 0);
		}},

		CppUtils::Test{"Language/Lexer/GrammarLexer/Expressions", [] {
			using namespace CppUtils::Type::Literals;
			auto grammarLexer = CppUtils::Language::Lexer::GrammarLexer<CppUtils::Type::Token>{};

			grammarLexer.addParsingFunction("spaceParser"_token, CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token>);
			grammarLexer.addParsingFunction("keywordParser"_token, CppUtils::Language::Parser::keywordParser<CppUtils::Type::Token>);
			grammarLexer.addParsingFunction("quoteParser"_token, CppUtils::Language::Parser::quoteParser<CppUtils::Type::Token>);
			
			static constexpr auto grammarSrc = R"(
			main: _instruction spaceParser;
			_instruction: function spaceParser ';';
			function: spaceParser keywordParser spaceParser '(' _argument spaceParser ')';
			_argument: string;
			string: spaceParser quoteParser;
			)"sv;
			const auto grammarTree = grammarLexer.parseGrammar(grammarSrc);
			CppUtils::Graph::logTreeNode(grammarTree);
			
			static constexpr auto languageSrc = R"(
				print("Hello World!");
			)"sv;
			const auto tokenTree = grammarLexer.parseLanguage("main"_token, languageSrc);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(std::get<CppUtils::Type::Token>(tokenTree.value) == "main"_token);
			ASSERT(tokenTree.childs.size() == 1);
			ASSERT(std::get<CppUtils::Type::Token>(tokenTree.childs.at(0).value) == "function"_token);
			ASSERT(tokenTree.childs.at(0).childs.size() == 2);
			ASSERT(std::get<CppUtils::Type::Token>(tokenTree.childs.at(0).childs.at(0).value) == "print"_token);
			ASSERT(std::get<CppUtils::Type::Token>(tokenTree.childs.at(0).childs.at(1).value) == "string"_token);
			ASSERT(tokenTree.childs.at(0).childs.at(1).childs.size() == 1);
			ASSERT(std::get<CppUtils::Type::Token>(tokenTree.childs.at(0).childs.at(1).childs.at(0).value) == "Hello World!"_token);
		}},

		CppUtils::Test{"Language/Lexer/GrammarLexer/Recurrence", [] {
			using namespace CppUtils::Type::Literals;
			auto grammarLexer = CppUtils::Language::Lexer::GrammarLexer<CppUtils::Type::Token>{};

			grammarLexer.addParsingFunction("spaceParser"_token, CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token>);
			grammarLexer.addParsingFunction("keywordParser"_token, CppUtils::Language::Parser::keywordParser<CppUtils::Type::Token>);
			
			static constexpr auto grammarSrc = R"(
			main: (_word >= 0) spaceParser;
			_word: spaceParser keywordParser;
			)"sv;
			const auto grammarTree = grammarLexer.parseGrammar(grammarSrc);
			CppUtils::Graph::logTreeNode(grammarTree);
			
			static constexpr auto languageSrc = "This is a test"sv;
			const auto tokenTree = grammarLexer.parseLanguage("main"_token, languageSrc);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(std::get<CppUtils::Type::Token>(tokenTree.value) == "main"_token);
			ASSERT(tokenTree.childs.size() == 4);
		}},

		CppUtils::Test{"Language/Lexer/GrammarLexer/Avanced", [] {
			using namespace CppUtils::Type::Literals;
			auto grammarLexer = CppUtils::Language::Lexer::GrammarLexer<CppUtils::Type::Token>{};

			grammarLexer.addParsingFunction("spaceParser"_token, CppUtils::Language::Parser::spaceParser<CppUtils::Type::Token>);
			grammarLexer.addParsingFunction("keywordParser"_token, CppUtils::Language::Parser::keywordParser<CppUtils::Type::Token>);
			grammarLexer.addParsingFunction("quoteParser"_token, CppUtils::Language::Parser::quoteParser<CppUtils::Type::Token>);
			
			static constexpr auto grammarSrc = R"(
			main: (_instruction >= 0) spaceParser;
			_instruction: (function || variableDeclaration) spaceParser ';';
			function: spaceParser keywordParser spaceParser '(' ~_arguments spaceParser ')';
			_arguments: _value ~_secondaryArgument;
			_secondaryArgument: spaceParser ',' _arguments;
			variableDeclaration: spaceParser keywordParser spaceParser '=' string;
			_value: (string || variableCall);
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

			ASSERT(std::get<CppUtils::Type::Token>(tokenTree.value) == "main"_token);
			ASSERT(tokenTree.childs.size() == 4);
		}}

	};
}
