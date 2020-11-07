#pragma once

#include <CppUtils.hpp>

namespace UnitTests::Language::Grammar
{
	const auto tests = std::vector<CppUtils::Test::UnitTest>{

		CppUtils::Test::UnitTest("Language/Grammar/Lexeme", [] {
			using namespace CppUtils::Type::Literals;
			CppUtils::Terminal::setConsoleOutputUTF8();
			
			auto grammarLexer = CppUtils::Language::GrammarLexer{};

			static constexpr auto grammarSrc = "\
			grammar: \"Test\";\
			"sv;
			const auto grammarTree = grammarLexer.parseGrammar(grammarSrc);
			CppUtils::Graph::logTreeNode(grammarTree);
			
			static constexpr auto languageSrc = "Test"sv;
			const auto tokenTree = grammarLexer.parseLanguage(languageSrc);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.self == "grammar"_typeId);
			ASSERT(tokenTree.childs.size() == 0);
		}),

		CppUtils::Test::UnitTest("Language/Grammar/Definitions", [] {
			using namespace CppUtils::Type::Literals;
			CppUtils::Terminal::setConsoleOutputUTF8();

			auto grammarLexer = CppUtils::Language::GrammarLexer{};

			grammarLexer.addParserFunction("spaceParser"_typeId, CppUtils::Language::Parser::spaceParser);
			grammarLexer.addParserFunction("keywordParser"_typeId, CppUtils::Language::Parser::keywordParser);
			grammarLexer.addParserFunction("quoteParser"_typeId, CppUtils::Language::Parser::quoteParser);
			
			static constexpr auto grammarSrc = "\
			grammar: _instruction spaceParser;\
			_instruction: function spaceParser ';';\
			function: spaceParser keywordParser spaceParser '(' _argument spaceParser ')';\
			_argument: string;\
			string: spaceParser quoteParser;\
			"sv;
			const auto grammarTree = grammarLexer.parseGrammar(grammarSrc);
			CppUtils::Graph::logTreeNode(grammarTree);
			
			static constexpr auto languageSrc = "print(\"Hello World!\");"sv;
			const auto tokenTree = grammarLexer.parseLanguage(languageSrc);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.self == "grammar"_typeId);
			ASSERT(tokenTree.childs.size() == 1);
			ASSERT(tokenTree.childs.at(0).self == "function"_typeId);
			ASSERT(tokenTree.childs.at(0).childs.size() == 2);
			ASSERT(tokenTree.childs.at(0).childs.at(0).self == "print"_typeId);
			ASSERT(tokenTree.childs.at(0).childs.at(1).self == "string"_typeId);
			ASSERT(tokenTree.childs.at(0).childs.at(1).childs.size() == 1);
			ASSERT(tokenTree.childs.at(0).childs.at(1).childs.at(0).self == "Hello World!"_typeId);
		}),

		CppUtils::Test::UnitTest("Language/Grammar/Recurrence", [] {
			using namespace CppUtils::Type::Literals;
			CppUtils::Terminal::setConsoleOutputUTF8();

			auto grammarLexer = CppUtils::Language::GrammarLexer{};

			grammarLexer.addParserFunction("spaceParser"_typeId, CppUtils::Language::Parser::spaceParser);
			grammarLexer.addParserFunction("keywordParser"_typeId, CppUtils::Language::Parser::keywordParser);
			
			static constexpr auto grammarSrc = "\
			grammar: (_word >= 0) spaceParser;\
			_word: spaceParser keywordParser;\
			"sv;
			const auto grammarTree = grammarLexer.parseGrammar(grammarSrc);
			CppUtils::Graph::logTreeNode(grammarTree);
			
			static constexpr auto languageSrc = "This is a test"sv;
			const auto tokenTree = grammarLexer.parseLanguage(languageSrc);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.self == "grammar"_typeId);
			ASSERT(tokenTree.childs.size() == 4);
		}),

		CppUtils::Test::UnitTest("Language/Grammar/Avanced", [] {
			using namespace CppUtils::Type::Literals;
			CppUtils::Terminal::setConsoleOutputUTF8();

			auto grammarLexer = CppUtils::Language::GrammarLexer{};

			grammarLexer.addParserFunction("spaceParser"_typeId, CppUtils::Language::Parser::spaceParser);
			grammarLexer.addParserFunction("keywordParser"_typeId, CppUtils::Language::Parser::keywordParser);
			grammarLexer.addParserFunction("quoteParser"_typeId, CppUtils::Language::Parser::quoteParser);
			
			static constexpr auto grammarSrc = "\
			grammar: (_instruction >= 0) spaceParser;\
			_instruction: (function || variableDeclaration) spaceParser ';';\
			function: spaceParser keywordParser spaceParser '(' !_arguments spaceParser ')';\
			_arguments: _value !_secondaryArgument;\
			_secondaryArgument: spaceParser ',' _arguments;\
			variableDeclaration: spaceParser keywordParser spaceParser '=' string;\
			_value: (string || variableCall);\
			string: spaceParser quoteParser;\
			variableCall: spaceParser keywordParser;\
			"sv;
			const auto grammarTree = grammarLexer.parseGrammar(grammarSrc);
			CppUtils::Graph::logTreeNode(grammarTree);
			
			static constexpr auto languageSrc = "\
			print(\"Hello World!\");\
			variable = \"azerty\";\
			print(\"Test\", variable, \"123\");\
			print(\"Ok\");"sv;
			const auto tokenTree = grammarLexer.parseLanguage(languageSrc);
			CppUtils::Graph::logTreeNode(tokenTree);

			ASSERT(tokenTree.self == "grammar"_typeId);
			ASSERT(tokenTree.childs.size() == 4);
		})

	};
}
