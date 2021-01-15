#pragma once

#include <CppUtils/Language/Parser/Cursor.hpp>
#include <CppUtils/Language/Parser/Parsers.hpp>
#include <CppUtils/Language/Lexer/Lexer.hpp>

namespace CppUtils::Language::Lexer
{
	class GrammarLexer final
	{
	public:
		GrammarLexer()
		{
			using namespace Type::Literals;

			auto& main = m_grammarLexer.newExpression("main"_token);
			auto& identifier = m_grammarLexer.newExpression("identifier"_token, false);
			auto& token = m_grammarLexer.newExpression("token"_token);
			auto& statement = m_grammarLexer.newExpression("statement"_token);
			auto& lexemes = m_grammarLexer.newExpression("lexemes"_token);
			auto& lexeme = m_grammarLexer.newExpression("lexeme"_token, false);
			auto& string = m_grammarLexer.newExpression("string"_token);
			auto& optional = m_grammarLexer.newExpression("optional"_token);
			auto& parenthesis = m_grammarLexer.newExpression("parenthesis"_token, false);
			auto& recurrence = m_grammarLexer.newExpression("recurrence"_token);
			auto& alternative = m_grammarLexer.newExpression("alternative"_token);
			auto& alternativeArgument = m_grammarLexer.newExpression("alternativeArgument"_token, false);
			auto& recurrenceOperator = m_grammarLexer.newExpression("recurrenceOperator"_token, false);
			auto& equalTo = m_grammarLexer.newExpression("equalTo"_token);
			auto& moreOrEqualTo = m_grammarLexer.newExpression("moreOrEqualTo"_token);
			auto& moreThan = m_grammarLexer.newExpression("moreThan"_token);
			
			main >> (statement >= 0) >> Parser::spaceParser;
			identifier >> Parser::spaceParser >> Parser::keywordParser;
			token >> identifier;
			statement
				>> identifier
				>> Parser::spaceParser >> ':'
				>> lexemes
				>> Parser::spaceParser >> ';';
			lexemes	>> (lexeme >= 1);
			lexeme >> Parser::spaceParser >> (string || token || optional || parenthesis);
			string >> Parser::quoteParser;
			optional
				>> '~'
				>> Parser::spaceParser >> lexeme;
			parenthesis
				>> '('
				>> Parser::spaceParser >> (recurrence || alternative || lexemes)
				>> Parser::spaceParser >> ')';
			recurrence
				>> Parser::spaceParser >> lexeme
				>> recurrenceOperator
				>> Parser::spaceParser >> Parser::uintParser;
			alternative
				>> Parser::spaceParser >> lexeme
				>> (alternativeArgument >= 1);
			alternativeArgument
				>> Parser::spaceParser >> "||"
				>> Parser::spaceParser >> lexeme;
			recurrenceOperator
				>> Parser::spaceParser >> (equalTo || moreOrEqualTo || moreThan);
			equalTo >> "*";
			moreOrEqualTo >> ">=";
			moreThan >> '>';
		}

		inline void addParserFunction(const Type::Token& token, Parser::ParserFunction parserFunction)
		{
			m_parserFunctions[token] = std::move(parserFunction);
		}

		Graph::TokenNode parseGrammar(std::string_view src)
		{
			using namespace Type::Literals;

			const auto tokenTree = m_grammarLexer.parse("main"_token, src);
			auto existingStatements = std::unordered_map<Type::Token, bool, Type::Token::hash_fn>{};

			for (const auto& statement : tokenTree.childs)
			{
				const auto& token = statement.childs.at(0).self;
				auto& expression = m_languageLexer.newExpression(token, token.name.at(0) != '_');
				existingStatements[token] = true;
				const auto& lexemes = statement.childs.at(1).childs;

				for (const auto& lexeme : lexemes)
				{
					const auto& lexemeType = lexeme.self;
					
					switch (lexemeType.id)
					{
						case "token"_token.id:
							parseToken(expression, lexeme.childs, existingStatements);
							break;
						case "string"_token.id:
							parseString(expression, lexeme.childs, existingStatements);
							break;
						case "optional"_token.id:
							parseOptional(expression, lexeme.childs, existingStatements);
							break;
						case "recurrence"_token.id:
							parseRecurrence(expression, lexeme.childs, existingStatements);
							break;
						case "alternative"_token.id:
							parseAlternative(expression, lexeme.childs, existingStatements);
							break;
						default:
							throw std::runtime_error{"Unknown lexeme type: " + std::string{lexemeType.name}};
					}
				}
			}
			for (const auto& [token, state] : existingStatements)
				if (!state)
					throw std::runtime_error{"Undefined expression: " + std::string{token.name}};
			return tokenTree;
		}

		[[nodiscard]] inline Graph::TokenNode parseLanguage(std::string_view src) const
		{
			using namespace Type::Literals;

			return m_languageLexer.parse("main"_token, src);
		}

	private:
		using ExistingStatements = std::unordered_map<Type::Token, bool, Type::Token::hash_fn>;

		void parseToken(Parser::Expression& expression, const std::vector<Graph::TokenNode>& attributes, ExistingStatements& existingStatements)
		{
			const auto& token = attributes.at(0).self;

			if (m_parserFunctions.find(token) != m_parserFunctions.end())
				expression >> m_parserFunctions.at(token);
			else
			{
				if (existingStatements.find(token) == existingStatements.end())
					existingStatements[token] = false;
				expression >> m_languageLexer.newExpression(token, token.name.at(0) != '_');
			}
		}

		void parseString(Parser::Expression& expression, const std::vector<Graph::TokenNode>& attributes, [[maybe_unused]] ExistingStatements& existingStatements)
		{
			expression >> std::string{attributes.at(0).self.name};
		}

		void parseOptional(Parser::Expression& expression, const std::vector<Graph::TokenNode>& attributes, [[maybe_unused]] ExistingStatements& existingStatements)
		{
			const auto& token = attributes.at(0).childs.at(0).self;
			const auto& tokenLexeme = m_languageLexer.newExpression(token, token.name.at(0) != '_');

			if (existingStatements.find(token) == existingStatements.end())
				existingStatements[token] = false;
			expression >> ~tokenLexeme;
		}

		void parseRecurrence(Parser::Expression& expression, const std::vector<Graph::TokenNode>& attributes, ExistingStatements& existingStatements)
		{
			using namespace Type::Literals;
			
			const auto& token = attributes.at(0).childs.at(0).self;
			const auto& recurrenceOperator = attributes.at(1).self;
			const auto longRepetitions = std::stoul(std::string{attributes.at(2).self.name});
			const auto repetitions = static_cast<unsigned int>(longRepetitions);
			const auto& tokenExpression = m_languageLexer.newExpression(token, token.name.at(0) != '_');
			
			if (repetitions != longRepetitions)
				throw std::out_of_range{"Number of repetitions too large"};
			if (existingStatements.find(token) == existingStatements.end())
				existingStatements[token] = false;
			switch (recurrenceOperator.id)
			{
				case "equalTo"_token.id:
					expression >> (tokenExpression * repetitions);
					break;
				case "moreOrEqualTo"_token.id:
					expression >> (tokenExpression >= repetitions);
					break;
				case "moreThan"_token.id:
					expression >> (tokenExpression > repetitions);
					break;
				default:
					throw std::runtime_error{"Unknown operator type: " + std::string{recurrenceOperator.name}};
			}
		}

		void parseAlternative(Parser::Expression& expression, const std::vector<Graph::TokenNode>& attributes, ExistingStatements& existingStatements)
		{
			auto tokens = std::vector<Type::Token>{};
			tokens.reserve(attributes.size());
			std::transform(attributes.begin(), attributes.end(), std::back_inserter(tokens), [](const auto& tokenNode) {
				return tokenNode.childs.at(0).self;
			});
			for (const auto& token : tokens)
				if (existingStatements.find(token) == existingStatements.end())
					existingStatements[token] = false;
			expression >> Parser::Alternative{std::move(tokens)};
		}

		std::unordered_map<Type::Token, Parser::ParserFunction, Type::Token::hash_fn> m_parserFunctions;
		Lexer m_grammarLexer;
		Lexer m_languageLexer;
	};
}
