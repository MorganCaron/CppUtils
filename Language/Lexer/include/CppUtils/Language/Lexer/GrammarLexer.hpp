#pragma once

#include <CppUtils/Language/Parser/Cursor.hpp>
#include <CppUtils/Language/Parser/Parsers.hpp>
#include <CppUtils/Language/Lexer/Lexer.hpp>

namespace CppUtils::Language::Lexer
{
	template<typename... Types>
	class GrammarLexer final
	{
	private:
		using GrammarLexerTreeNode = Graph::VariantTreeNode<Type::Token, unsigned int>;

	public:
		using LanguageTreeNode = Graph::VariantTreeNode<Types...>;

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
			auto& tag = m_grammarLexer.newExpression("tag"_token);
			auto& optional = m_grammarLexer.newExpression("optional"_token);
			auto& parenthesis = m_grammarLexer.newExpression("parenthesis"_token, false);
			auto& recurrence = m_grammarLexer.newExpression("recurrence"_token);
			auto& alternative = m_grammarLexer.newExpression("alternative"_token);
			auto& alternativeArgument = m_grammarLexer.newExpression("alternativeArgument"_token, false);
			auto& recurrenceOperator = m_grammarLexer.newExpression("recurrenceOperator"_token, false);
			auto& equalTo = m_grammarLexer.newExpression("equalTo"_token);
			auto& moreOrEqualTo = m_grammarLexer.newExpression("moreOrEqualTo"_token);
			auto& moreThan = m_grammarLexer.newExpression("moreThan"_token);
			
			main >> (statement >= 0) >> Parser::spaceParser<Type::Token, unsigned int>;
			identifier >> Parser::spaceParser<Type::Token, unsigned int> >> Parser::keywordParser<Type::Token, unsigned int>;
			token >> identifier;
			statement
				>> identifier
				>> Parser::spaceParser<Type::Token, unsigned int> >> ':'
				>> lexemes
				>> Parser::spaceParser<Type::Token, unsigned int> >> ';';
			lexemes	>> (lexeme >= 1);
			lexeme >> Parser::spaceParser<Type::Token, unsigned int> >> (string || token || tag || optional || parenthesis);
			string >> Parser::quoteParser<Type::Token, unsigned int>;
			tag
				>> '['
				>> Parser::spaceParser<Type::Token, unsigned int> >> identifier
				>> Parser::spaceParser<Type::Token, unsigned int> >> ']';
			optional
				>> '~'
				>> Parser::spaceParser<Type::Token, unsigned int> >> lexeme;
			parenthesis
				>> '('
				>> Parser::spaceParser<Type::Token, unsigned int> >> (recurrence || alternative || lexemes)
				>> Parser::spaceParser<Type::Token, unsigned int> >> ')';
			recurrence
				>> Parser::spaceParser<Type::Token, unsigned int> >> lexeme
				>> recurrenceOperator
				>> Parser::spaceParser<Type::Token, unsigned int> >> Parser::uintParser<Type::Token, unsigned int>;
			alternative
				>> Parser::spaceParser<Type::Token, unsigned int> >> lexeme
				>> (alternativeArgument >= 1);
			alternativeArgument
				>> Parser::spaceParser<Type::Token, unsigned int> >> "||"
				>> Parser::spaceParser<Type::Token, unsigned int> >> lexeme;
			recurrenceOperator
				>> Parser::spaceParser<Type::Token, unsigned int> >> (equalTo || moreOrEqualTo || moreThan);
			equalTo >> "*";
			moreOrEqualTo >> ">=";
			moreThan >> '>';
		}

		inline void addParsingFunction(const Type::Token& token, Parser::ParsingFunction<Types...> parsingFunction)
		{
			m_parsingFunctions[token] = std::move(parsingFunction);
		}

		GrammarLexerTreeNode parseGrammar(std::string_view src)
		{
			using namespace Type::Literals;

			auto tokenTree = m_grammarLexer.parseString("main"_token, src);
			auto existingStatements = std::unordered_map<Type::Token, bool, Type::Token::hash_fn>{};

			for (const auto& statement : tokenTree.childs)
			{
				const auto& token = std::get<Type::Token>(statement.childs.at(0).value);
				auto& expression = m_languageLexer.newExpression(token, token.name.at(0) != '_');
				existingStatements[token] = true;
				const auto& lexemes = statement.childs.at(1).childs;

				for (const auto& lexeme : lexemes)
				{
					const auto& lexemeType = std::get<Type::Token>(lexeme.value);
					
					switch (lexemeType.id)
					{
						case "token"_token.id:
							parseToken(expression, lexeme.childs, existingStatements);
							break;
						case "string"_token.id:
							parseString(expression, lexeme.childs, existingStatements);
							break;
						case "tag"_token.id:
							parseName(expression, lexeme.childs, existingStatements);
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

		[[nodiscard]] inline Graph::VariantTreeNode<Types...> parseLanguage(const Type::Token& token, std::string_view src) const
		{
			return m_languageLexer.parseString(token, src);
		}

		inline void parseSegment(const Type::Token& token, Parser::Context<Types...>& context) const
		{
			m_languageLexer.parseSegment(token, context);
		}

	private:
		using ExistingStatements = std::unordered_map<Type::Token, bool, Type::Token::hash_fn>;

		void parseToken(Parser::Expression<Types...>& expression, const std::vector<GrammarLexerTreeNode>& attributes, ExistingStatements& existingStatements)
		{
			const auto& token = std::get<Type::Token>(attributes.at(0).value);

			if (m_parsingFunctions.find(token) != m_parsingFunctions.end())
				expression >> m_parsingFunctions.at(token);
			else
			{
				if (existingStatements.find(token) == existingStatements.end())
					existingStatements[token] = false;
				expression >> m_languageLexer.newExpression(token, token.name.at(0) != '_');
			}
		}

		void parseString(Parser::Expression<Types...>& expression, const std::vector<GrammarLexerTreeNode>& attributes, [[maybe_unused]] ExistingStatements& existingStatements)
		{
			expression >> std::string{std::get<Type::Token>(attributes.at(0).value).name};
		}

		void parseName(Parser::Expression<Types...>& expression, const std::vector<GrammarLexerTreeNode>& attributes, [[maybe_unused]] ExistingStatements& existingStatements)
		{
			const auto& token = std::get<Type::Token>(attributes.at(0).value);

			if (m_parsingFunctions.find(token) == m_parsingFunctions.end())
				throw std::runtime_error{"Unknown parsing function: " + std::string{token.name}};
			expression >> Parser::TagLexeme<Types...>{m_parsingFunctions.at(token)};
		}

		void parseOptional(Parser::Expression<Types...>& expression, const std::vector<GrammarLexerTreeNode>& attributes, [[maybe_unused]] ExistingStatements& existingStatements)
		{
			const auto& token = std::get<Type::Token>(attributes.at(0).childs.at(0).value);
			const auto& tokenLexeme = m_languageLexer.newExpression(token, token.name.at(0) != '_');

			if (existingStatements.find(token) == existingStatements.end())
				existingStatements[token] = false;
			expression >> ~tokenLexeme;
		}

		void parseRecurrence(Parser::Expression<Types...>& expression, const std::vector<GrammarLexerTreeNode>& attributes, ExistingStatements& existingStatements)
		{
			using namespace Type::Literals;
			
			const auto& token = std::get<Type::Token>(attributes.at(0).childs.at(0).value);
			const auto& recurrenceOperator = std::get<Type::Token>(attributes.at(1).value);
			const auto repetitions = std::get<unsigned int>(attributes.at(2).value);
			const auto& tokenExpression = m_languageLexer.newExpression(token, token.name.at(0) != '_');
			
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

		void parseAlternative(Parser::Expression<Types...>& expression, const std::vector<GrammarLexerTreeNode>& attributes, ExistingStatements& existingStatements)
		{
			auto tokens = std::vector<Type::Token>{};
			tokens.reserve(attributes.size());
			std::transform(attributes.begin(), attributes.end(), std::back_inserter(tokens), [](const auto& tokenNode) {
				return std::get<Type::Token>(tokenNode.childs.at(0).value);
			});
			for (const auto& token : tokens)
				if (existingStatements.find(token) == existingStatements.end())
					existingStatements[token] = false;
			expression >> Parser::Alternative{std::move(tokens)};
		}

		std::unordered_map<Type::Token, Parser::ParsingFunction<Types...>, Type::Token::hash_fn> m_parsingFunctions;
		Lexer<Type::Token, unsigned int> m_grammarLexer;
		Lexer<Types...> m_languageLexer;
	};
}
