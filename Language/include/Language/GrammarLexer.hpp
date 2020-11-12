#pragma once

#include <Language/Lexer.hpp>
#include <Language/Parser.hpp>

namespace CppUtils::Language
{
	class GrammarLexer final
	{
	public:
		GrammarLexer()
		{
			auto& main = m_grammarLexer.lexeme("main"_typeId);
			auto& statement = m_grammarLexer.lexeme("statement"_typeId);
			auto& lexemes = m_grammarLexer.lexeme("lexemes"_typeId);
			auto& lexeme = m_grammarLexer.lexeme("lexeme"_typeId, false);
			auto& string = m_grammarLexer.lexeme("string"_typeId);
			auto& token = m_grammarLexer.lexeme("token"_typeId);
			auto& optional = m_grammarLexer.lexeme("optional"_typeId);
			auto& parenthesis = m_grammarLexer.lexeme("parenthesis"_typeId, false);
			auto& recurrence = m_grammarLexer.lexeme("recurrence"_typeId);
			auto& contingence = m_grammarLexer.lexeme("contingence"_typeId);
			auto& contingenceArgument = m_grammarLexer.lexeme("contingenceArgument"_typeId, false);
			auto& recurrenceOperator = m_grammarLexer.lexeme("recurrenceOperator"_typeId, false);
			auto& equalTo = m_grammarLexer.lexeme("equalTo"_typeId);
			auto& moreOrEqualTo = m_grammarLexer.lexeme("moreOrEqualTo"_typeId);
			auto& moreThan = m_grammarLexer.lexeme("moreThan"_typeId);
			
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

		inline void addParserFunction(const Lexeme::Token& token, Lexeme::ParserFunction parserFunction)
		{
			m_parserFunctions[token] = std::move(parserFunction);
		}

		Lexeme::TokenNode parseGrammar(std::string_view src)
		{
			const auto tokenTree = m_grammarLexer.parse("main"_typeId, src);
			auto existingStatements = std::unordered_map<Lexeme::Token, bool, Lexeme::Token::hash_fn>{};

			for (const auto& statement : tokenTree.childs)
			{
				const auto& token = statement.childs.at(0).self;
				auto& tokenLexeme = m_languageLexer.lexeme(token, token.name.at(0) != '_');
				existingStatements[token] = true;
				const auto& lexemes = statement.childs.at(1).childs;

				for (const auto& lexeme : lexemes)
				{
					const auto& lexemeType = lexeme.self;
					
					switch (lexemeType.id)
					{
						case "token"_typeId.id:
							parseToken(tokenLexeme, lexeme.childs, existingStatements);
							break;
						case "string"_typeId.id:
							parseString(tokenLexeme, lexeme.childs, existingStatements);
							break;
						case "optional"_typeId.id:
							parseOptional(tokenLexeme, lexeme.childs, existingStatements);
							break;
						case "recurrence"_typeId.id:
							parseRecurrence(tokenLexeme, lexeme.childs, existingStatements);
							break;
						case "contingence"_typeId.id:
							parseContingence(tokenLexeme, lexeme.childs, existingStatements);
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

		[[nodiscard]] inline Lexeme::TokenNode parseLanguage(std::string_view src) const
		{
			return m_languageLexer.parse("main"_typeId, src);
		}

	private:
		using ExistingStatements = std::unordered_map<Lexeme::Token, bool, Lexeme::Token::hash_fn>;

		void parseToken(Lexeme::Definition& definition, const std::vector<Lexeme::TokenNode>& attributes, ExistingStatements& existingStatements)
		{
			const auto& token = attributes.at(0).self;

			if (m_parserFunctions.find(token) != m_parserFunctions.end())
				definition >> m_parserFunctions.at(token);
			else
			{
				if (existingStatements.find(token) == existingStatements.end())
					existingStatements[token] = false;
				definition >> m_languageLexer.lexeme(token, token.name.at(0) != '_');
			}
		}

		void parseString(Lexeme::Definition& definition, const std::vector<Lexeme::TokenNode>& attributes, [[maybe_unused]] ExistingStatements& existingStatements)
		{
			definition >> std::string{attributes.at(0).self.name};
		}

		void parseOptional(Lexeme::Definition& definition, const std::vector<Lexeme::TokenNode>& attributes, [[maybe_unused]] ExistingStatements& existingStatements)
		{
			const auto& token = attributes.at(0).childs.at(0).self;
			const auto& tokenLexeme = m_languageLexer.lexeme(token, token.name.at(0) != '_');

			if (existingStatements.find(token) == existingStatements.end())
				existingStatements[token] = false;
			definition >> !tokenLexeme;
		}

		void parseRecurrence(Lexeme::Definition& definition, const std::vector<Lexeme::TokenNode>& attributes, ExistingStatements& existingStatements)
		{
			const auto& token = attributes.at(0).childs.at(0).self;
			const auto& recurrenceOperator = attributes.at(1).self;
			const auto longRepetitions = std::stoul(std::string{attributes.at(2).self.name});
			const auto repetitions = static_cast<unsigned int>(longRepetitions);
			const auto& tokenLexeme = m_languageLexer.lexeme(token, token.name.at(0) != '_');
			
			if (repetitions != longRepetitions)
				throw std::out_of_range{"Number of repetitions too large"};
			if (existingStatements.find(token) == existingStatements.end())
				existingStatements[token] = false;
			switch (recurrenceOperator.id)
			{
				case "equalTo"_typeId.id:
					definition >> (tokenLexeme * repetitions);
					break;
				case "moreOrEqualTo"_typeId.id:
					definition >> (tokenLexeme >= repetitions);
					break;
				case "moreThan"_typeId.id:
					definition >> (tokenLexeme > repetitions);
					break;
				default:
					throw std::runtime_error{"Unknown operator type: " + std::string{recurrenceOperator.name}};
			}
		}

		void parseContingence(Lexeme::Definition& definition, const std::vector<Lexeme::TokenNode>& attributes, ExistingStatements& existingStatements)
		{
			auto tokens = std::vector<Lexeme::Token>{};
			tokens.reserve(attributes.size());
			std::transform(attributes.begin(), attributes.end(), std::back_inserter(tokens), [](const auto& tokenNode) {
				return tokenNode.childs.at(0).self;
			});
			for (const auto& token : tokens)
				if (existingStatements.find(token) == existingStatements.end())
					existingStatements[token] = false;
			definition >> Lexeme::Contingence{std::move(tokens)};
		}

		std::unordered_map<Lexeme::Token, Lexeme::ParserFunction, Lexeme::Token::hash_fn> m_parserFunctions;
		Lexer m_grammarLexer;
		Lexer m_languageLexer;
	};
}
