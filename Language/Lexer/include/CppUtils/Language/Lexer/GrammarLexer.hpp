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

			auto& main = m_grammarLexer.expression("main"_typeId);
			auto& statement = m_grammarLexer.expression("statement"_typeId);
			auto& lexemes = m_grammarLexer.expression("lexemes"_typeId);
			auto& lexeme = m_grammarLexer.expression("lexeme"_typeId, false);
			auto& string = m_grammarLexer.expression("string"_typeId);
			auto& token = m_grammarLexer.expression("token"_typeId);
			auto& optional = m_grammarLexer.expression("optional"_typeId);
			auto& parenthesis = m_grammarLexer.expression("parenthesis"_typeId, false);
			auto& recurrence = m_grammarLexer.expression("recurrence"_typeId);
			auto& contingence = m_grammarLexer.expression("contingence"_typeId);
			auto& contingenceArgument = m_grammarLexer.expression("contingenceArgument"_typeId, false);
			auto& recurrenceOperator = m_grammarLexer.expression("recurrenceOperator"_typeId, false);
			auto& equalTo = m_grammarLexer.expression("equalTo"_typeId);
			auto& moreOrEqualTo = m_grammarLexer.expression("moreOrEqualTo"_typeId);
			auto& moreThan = m_grammarLexer.expression("moreThan"_typeId);
			
			main >> (statement >= 0) >> Parser::spaceParser;
			statement
				>> Parser::spaceParser >> Parser::keywordParser
				>> Parser::spaceParser >> ':'
				>> lexemes
				>> Parser::spaceParser >> ';';
			lexemes	>> (lexeme >= 1);
			lexeme
				>> Parser::spaceParser >> (string || token || optional || parenthesis);
			string >> Parser::quoteParser;
			token >> Parser::keywordParser;
			optional
				>> '!'
				>> Parser::spaceParser >> token;
			parenthesis
				>> '('
				>> Parser::spaceParser >> (recurrence || contingence)
				>> Parser::spaceParser >> ')';
			recurrence
				>> Parser::spaceParser >> token
				>> recurrenceOperator
				>> Parser::spaceParser >> Parser::uintParser;
			contingence
				>> Parser::spaceParser >> token
				>> (contingenceArgument >= 1);
			contingenceArgument
				>> Parser::spaceParser >> "||"
				>> Parser::spaceParser >> token;
			recurrenceOperator
				>> Parser::spaceParser >> (equalTo || moreOrEqualTo || moreThan);
			equalTo >> "*";
			moreOrEqualTo >> ">=";
			moreThan >> '>';
		}

		inline void addParserFunction(const Parser::Token& token, Parser::ParserFunction parserFunction)
		{
			m_parserFunctions[token] = std::move(parserFunction);
		}

		Parser::TokenNode parseGrammar(std::string_view src)
		{
			using namespace Type::Literals;

			const auto tokenTree = m_grammarLexer.parse("main"_typeId, src);
			auto existingStatements = std::unordered_map<Parser::Token, bool, Parser::Token::hash_fn>{};

			for (const auto& statement : tokenTree.childs)
			{
				const auto& token = statement.childs.at(0).self;
				auto& expression = m_languageLexer.expression(token, token.name.at(0) != '_');
				existingStatements[token] = true;
				const auto& lexemes = statement.childs.at(1).childs;

				for (const auto& lexeme : lexemes)
				{
					const auto& lexemeType = lexeme.self;
					
					switch (lexemeType.id)
					{
						case "token"_typeId.id:
							parseToken(expression, lexeme.childs, existingStatements);
							break;
						case "string"_typeId.id:
							parseString(expression, lexeme.childs, existingStatements);
							break;
						case "optional"_typeId.id:
							parseOptional(expression, lexeme.childs, existingStatements);
							break;
						case "recurrence"_typeId.id:
							parseRecurrence(expression, lexeme.childs, existingStatements);
							break;
						case "contingence"_typeId.id:
							parseContingence(expression, lexeme.childs, existingStatements);
							break;
						default:
							throw std::runtime_error{"Unknown token type: " + std::string{lexemeType.name}};
					}
				}
			}
			for (const auto& [token, state] : existingStatements)
				if (!state)
					throw std::runtime_error{"Undefined token: " + std::string{token.name}};
			return tokenTree;
		}

		[[nodiscard]] inline Parser::TokenNode parseLanguage(std::string_view src) const
		{
			using namespace Type::Literals;

			return m_languageLexer.parse("main"_typeId, src);
		}

	private:
		using ExistingStatements = std::unordered_map<Parser::Token, bool, Parser::Token::hash_fn>;

		void parseToken(Parser::Expression& expression, const std::vector<Parser::TokenNode>& attributes, ExistingStatements& existingStatements)
		{
			const auto& token = attributes.at(0).self;

			if (m_parserFunctions.find(token) != m_parserFunctions.end())
				expression >> m_parserFunctions.at(token);
			else
			{
				if (existingStatements.find(token) == existingStatements.end())
					existingStatements[token] = false;
				expression >> m_languageLexer.expression(token, token.name.at(0) != '_');
			}
		}

		void parseString(Parser::Expression& expression, const std::vector<Parser::TokenNode>& attributes, [[maybe_unused]] ExistingStatements& existingStatements)
		{
			expression >> std::string{attributes.at(0).self.name};
		}

		void parseOptional(Parser::Expression& expression, const std::vector<Parser::TokenNode>& attributes, [[maybe_unused]] ExistingStatements& existingStatements)
		{
			const auto& token = attributes.at(0).childs.at(0).self;
			const auto& tokenLexeme = m_languageLexer.expression(token, token.name.at(0) != '_');

			if (existingStatements.find(token) == existingStatements.end())
				existingStatements[token] = false;
			expression >> !tokenLexeme;
		}

		void parseRecurrence(Parser::Expression& expression, const std::vector<Parser::TokenNode>& attributes, ExistingStatements& existingStatements)
		{
			using namespace Type::Literals;
			
			const auto& token = attributes.at(0).childs.at(0).self;
			const auto& recurrenceOperator = attributes.at(1).self;
			const auto longRepetitions = std::stoul(std::string{attributes.at(2).self.name});
			const auto repetitions = static_cast<unsigned int>(longRepetitions);
			const auto& tokenExpression = m_languageLexer.expression(token, token.name.at(0) != '_');
			
			if (repetitions != longRepetitions)
				throw std::out_of_range{"Number of repetitions too large"};
			if (existingStatements.find(token) == existingStatements.end())
				existingStatements[token] = false;
			switch (recurrenceOperator.id)
			{
				case "equalTo"_typeId.id:
					expression >> (tokenExpression * repetitions);
					break;
				case "moreOrEqualTo"_typeId.id:
					expression >> (tokenExpression >= repetitions);
					break;
				case "moreThan"_typeId.id:
					expression >> (tokenExpression > repetitions);
					break;
				default:
					throw std::runtime_error{"Unknown operator type: " + std::string{recurrenceOperator.name}};
			}
		}

		void parseContingence(Parser::Expression& expression, const std::vector<Parser::TokenNode>& attributes, ExistingStatements& existingStatements)
		{
			auto tokens = std::vector<Parser::Token>{};
			tokens.reserve(attributes.size());
			std::transform(attributes.begin(), attributes.end(), std::back_inserter(tokens), [](const auto& tokenNode) {
				return tokenNode.childs.at(0).self;
			});
			for (const auto& token : tokens)
				if (existingStatements.find(token) == existingStatements.end())
					existingStatements[token] = false;
			expression >> Parser::Contingence{std::move(tokens)};
		}

		std::unordered_map<Parser::Token, Parser::ParserFunction, Parser::Token::hash_fn> m_parserFunctions;
		Lexer m_grammarLexer;
		Lexer m_languageLexer;
	};
}
