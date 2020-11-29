#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Lexer::GrammarLexer
{
	using namespace std::literals;
	
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest{"Language/Lexer/GrammarLexer/Lexeme", [] {
			using namespace CppUtils::Type::Literals;
			CppUtils::Terminal::setConsoleOutputUTF8();
			
			auto grammarLexer = CppUtils::Language::Lexer::GrammarLexer{};

			static constexpr auto grammarSrc = R"(
			main: "Test";
			)"sv;
			const auto grammarTree = grammarLexer.parseGrammar(grammarSrc);
			CppUtils::Graph::logTreeNode(grammarTree);
			
			static constexpr auto languageSrc = "Test"sv;
			const auto tokenTree = grammarLexer.parseLanguage(languageSrc);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.self == "main"_typeId);
			ASSERT(tokenTree.childs.size() == 0);
		}},

		CppUtils::Test::UnitTest{"Language/Lexer/GrammarLexer/Definitions", [] {
			using namespace CppUtils::Type::Literals;
			CppUtils::Terminal::setConsoleOutputUTF8();

			auto grammarLexer = CppUtils::Language::Lexer::GrammarLexer{};

			grammarLexer.addParserFunction("spaceParser"_typeId, CppUtils::Language::Lexer::Parsers::spaceParser);
			grammarLexer.addParserFunction("keywordParser"_typeId, CppUtils::Language::Lexer::Parsers::keywordParser);
			grammarLexer.addParserFunction("quoteParser"_typeId, CppUtils::Language::Lexer::Parsers::quoteParser);
			
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
			const auto tokenTree = grammarLexer.parseLanguage(languageSrc);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.self == "main"_typeId);
			ASSERT(tokenTree.childs.size() == 1);
			ASSERT(tokenTree.childs.at(0).self == "function"_typeId);
			ASSERT(tokenTree.childs.at(0).childs.size() == 2);
			ASSERT(tokenTree.childs.at(0).childs.at(0).self == "print"_typeId);
			ASSERT(tokenTree.childs.at(0).childs.at(1).self == "string"_typeId);
			ASSERT(tokenTree.childs.at(0).childs.at(1).childs.size() == 1);
			ASSERT(tokenTree.childs.at(0).childs.at(1).childs.at(0).self == "Hello World!"_typeId);
		}},

		CppUtils::Test::UnitTest{"Language/Lexer/GrammarLexer/Recurrence", [] {
			using namespace CppUtils::Type::Literals;
			CppUtils::Terminal::setConsoleOutputUTF8();

			auto grammarLexer = CppUtils::Language::Lexer::GrammarLexer{};

			grammarLexer.addParserFunction("spaceParser"_typeId, CppUtils::Language::Lexer::Parsers::spaceParser);
			grammarLexer.addParserFunction("keywordParser"_typeId, CppUtils::Language::Lexer::Parsers::keywordParser);
			
			static constexpr auto grammarSrc = R"(
			main: (_word >= 0) spaceParser;
			_word: spaceParser keywordParser;
			)"sv;
			const auto grammarTree = grammarLexer.parseGrammar(grammarSrc);
			CppUtils::Graph::logTreeNode(grammarTree);
			
			static constexpr auto languageSrc = "This is a test"sv;
			const auto tokenTree = grammarLexer.parseLanguage(languageSrc);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.self == "main"_typeId);
			ASSERT(tokenTree.childs.size() == 4);
		}},

		CppUtils::Test::UnitTest{"Language/Lexer/GrammarLexer/Avanced", [] {
			using namespace CppUtils::Type::Literals;
			CppUtils::Terminal::setConsoleOutputUTF8();

			auto grammarLexer = CppUtils::Language::Lexer::GrammarLexer{};

			grammarLexer.addParserFunction("spaceParser"_typeId, CppUtils::Language::Lexer::Parsers::spaceParser);
			grammarLexer.addParserFunction("keywordParser"_typeId, CppUtils::Language::Lexer::Parsers::keywordParser);
			grammarLexer.addParserFunction("quoteParser"_typeId, CppUtils::Language::Lexer::Parsers::quoteParser);
			
			static constexpr auto grammarSrc = R"(
			main: (_instruction >= 0) spaceParser;
			_instruction: (function || variableDeclaration) spaceParser ';';
			function: spaceParser keywordParser spaceParser '(' !_arguments spaceParser ')';
			_arguments: _value !_secondaryArgument;
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
			const auto tokenTree = grammarLexer.parseLanguage(languageSrc);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.self == "main"_typeId);
			ASSERT(tokenTree.childs.size() == 4);
		}}

	};
}
