#pragma once

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>

namespace CppUtils::Language::Lexer
{
	class StringTreeLexer final
	{
	public:
		StringTreeLexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;

			m_grammarLexer.addParserFunction("spaceParser"_token, Parser::spaceParser);
			m_grammarLexer.addParserFunction("quoteParser"_token, Parser::quoteParser);

			static constexpr auto grammarSrc = R"(
			main: (node >= 0) spaceParser;
			node: spaceParser quoteParser ~childs;
			childs: spaceParser '{' (node >= 1) spaceParser '}';
			)"sv;
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] inline Graph::TokenNode parse(const std::string_view src) const
		{
			using namespace Type::Literals;

			const auto tokenTree = m_grammarLexer.parseLanguage(src);
			auto finalTree = Graph::TokenNode{"main"_token};
			for (const auto& node : tokenTree.childs)
				finalTree.childs.emplace_back(generateNode(node));

			return finalTree;
		}

	private:
		static Graph::TokenNode generateNode(const Graph::TokenNode& srcNode)
		{
			auto destNode = srcNode.childs.at(0);
			if (srcNode.childs.size() == 2)
				for (const auto& childNode : srcNode.childs.at(1).childs)
					destNode.childs.emplace_back(generateNode(childNode));
			return destNode;
		}

		GrammarLexer m_grammarLexer;
	};

	namespace StringTree
	{
		[[nodiscard]] inline Graph::TokenNode parse(const std::string_view src)
		{
			static const auto stringTreeLexer = CppUtils::Language::Lexer::StringTreeLexer{};
			return stringTreeLexer.parse(src);
		}
	}
}
