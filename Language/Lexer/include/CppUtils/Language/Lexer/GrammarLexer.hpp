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
		using GrammarLexerTreeNode = Parser::ASTNode<Type::Token, unsigned int>;

	public:
		GrammarLexer()
		{
			using namespace Type::Literals;
			auto& main = m_grammarLexer.newExpression("main"_token, "root"_token);
			auto& identifier = m_grammarLexer.newExpression("identifier"_token, false);
			auto& token = m_grammarLexer.newExpression("token"_token);
			auto& statement = m_grammarLexer.newExpression("statement"_token, false);
			auto& hideStatement = m_grammarLexer.newExpression("hideStatement"_token);
			auto& statementName = m_grammarLexer.newExpression("statementName"_token);
			auto& lexemes = m_grammarLexer.newExpression("lexemes"_token, false);
			auto& lexeme = m_grammarLexer.newExpression("lexeme"_token, false);
			auto& string = m_grammarLexer.newExpression("string"_token);
			auto& tag = m_grammarLexer.newExpression("tag"_token);
			auto& muted = m_grammarLexer.newExpression("muted"_token);
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
				>> ~hideStatement
				>> Parser::TagLexeme{std::make_unique<Parser::TokenLexeme>(identifier.token)}
				>> ~statementName
				>> Parser::spaceParser<Type::Token, unsigned int> >> ':'
				>> lexemes
				>> Parser::spaceParser<Type::Token, unsigned int> >> ';';
			hideStatement >> Parser::spaceParser<Type::Token, unsigned int> >> '!';
			statementName
				>> Parser::spaceParser<Type::Token, unsigned int> >> '['
				>> identifier
				>> Parser::spaceParser<Type::Token, unsigned int> >> ']';
			lexemes	>> (lexeme >= 1);
			lexeme >> Parser::spaceParser<Type::Token, unsigned int> >> std::move(string || token || tag || muted || optional || parenthesis);
			string >> Parser::quoteParser<Type::Token, unsigned int>;
			tag
				>> '['
				>> identifier
				>> Parser::spaceParser<Type::Token, unsigned int> >> ']';
			muted >> '!' >> lexeme;
			optional >> '~' >> lexeme;
			parenthesis
				>> '('
				>> Parser::spaceParser<Type::Token, unsigned int> >> std::move(recurrence || alternative || lexemes)
				>> Parser::spaceParser<Type::Token, unsigned int> >> ')';
			recurrence
				>> lexeme
				>> recurrenceOperator
				>> Parser::spaceParser<Type::Token, unsigned int> >> Parser::uintParser<Type::Token, unsigned int>;
			alternative >> lexeme >> (alternativeArgument >= 1);
			alternativeArgument
				>> Parser::spaceParser<Type::Token, unsigned int> >> "||"
				>> lexeme;
			recurrenceOperator
				>> Parser::spaceParser<Type::Token, unsigned int> >> std::move(equalTo || moreOrEqualTo || moreThan);
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
			try
			{
				using namespace Type::Literals;
				auto tokenTree = m_grammarLexer.parseString("main"_token, src);
				createStatements(tokenTree);
				for (const auto& statement : tokenTree.childs)
				{
					auto& expression = m_languageLexer.getExpression(std::get<Type::Token>(statement.value));
					for (const auto& lexeme : statement.childs)
						if (std::get<Type::Token>(lexeme.value) != "hideStatement"_token && std::get<Type::Token>(lexeme.value) != "statementName"_token)
							expression.lexemes.emplace_back(parseLexeme(lexeme));
				}
				return tokenTree;
			}
			catch (const std::exception& exception)
			{
				using namespace std::literals;
				throw std::runtime_error{"In grammar parsing:\n"s + exception.what()};
			}
		}

		[[nodiscard]] inline Parser::ASTNode<Types...> parseLanguage(const Type::Token& token, std::string_view src) const
		{
			return m_languageLexer.parseString(token, src);
		}

		inline void parseSegment(const Type::Token& token, Parser::Context<Types...>& context) const
		{
			m_languageLexer.parseSegment(token, context);
		}

	private:
		void createStatements(const GrammarLexerTreeNode& tokenTree)
		{
			using namespace Type::Literals;
			for (const auto& statement : tokenTree.childs)
			{
				const auto& token = std::get<Type::Token>(statement.value);
				const auto& lexemes = statement.childs;
				const auto nbLexemes = lexemes.size();
				const auto isHidden = (nbLexemes > 0 && std::get<Type::Token>(lexemes.at(0).value) == "hideStatement"_token);
				const auto hasDifferentName = (nbLexemes > 0 && std::get<Type::Token>(lexemes.at(0).value) == "statementName"_token);
				const auto name = (isHidden ? ""_token : (hasDifferentName ? std::get<Type::Token>(lexemes.at(0).childs.at(0).value) : token));
				if (nbLexemes > 1 && std::get<Type::Token>(lexemes.at(1).value) == "statementName"_token)
					throw std::runtime_error{"A muted expression cannot have a name. Remove the '!' before the expression " + std::string{token.name}};
				m_languageLexer.createExpression(token, std::move(name));
			}
		}

		[[nodiscard]] std::unique_ptr<Parser::ILexeme> parseLexeme(const GrammarLexerTreeNode& lexeme)
		{
			using namespace Type::Literals;
			const auto& lexemeType = std::get<Type::Token>(lexeme.value);		
			switch (lexemeType.id)
			{
				case "token"_token.id:
					return parseToken(lexeme.childs);
				case "string"_token.id:
					return parseString(lexeme.childs);
				case "tag"_token.id:
					return parseTag(lexeme.childs);
				case "muted"_token.id:
					return parseMuted(lexeme.childs);
				case "optional"_token.id:
					return parseOptional(lexeme.childs);
				case "recurrence"_token.id:
					return parseRecurrence(lexeme.childs);
				case "alternative"_token.id:
					return parseAlternative(lexeme.childs);
				default:
					throw std::runtime_error{"Unknown lexeme type: " + std::string{lexemeType.name}};
			}
		}

		[[nodiscard]] std::unique_ptr<Parser::ILexeme> parseToken(const std::vector<GrammarLexerTreeNode>& attributes)
		{
			const auto& token = std::get<Type::Token>(attributes.at(0).value);
			if (m_parsingFunctions.find(token) != m_parsingFunctions.end())
				return std::make_unique<Parser::ParserLexeme<Types...>>(m_parsingFunctions.at(token));
			else
				return std::make_unique<Parser::TokenLexeme>(m_languageLexer.getExpression(token).token);
		}

		[[nodiscard]] std::unique_ptr<Parser::ILexeme> parseString(const std::vector<GrammarLexerTreeNode>& attributes)
		{
			return std::make_unique<Parser::StringLexeme>(std::string{std::get<Type::Token>(attributes.at(0).value).name});
		}

		[[nodiscard]] std::unique_ptr<Parser::ILexeme> parseTag(const std::vector<GrammarLexerTreeNode>& attributes)
		{
			const auto& token = std::get<Type::Token>(attributes.at(0).value);
			if (m_parsingFunctions.find(token) != m_parsingFunctions.end())
				throw std::runtime_error{"Error in tag \"" + std::string{token.name} + "\": Tags only accept tokens, not parsing functions"};
			return std::make_unique<Parser::TagLexeme>(std::make_unique<Parser::TokenLexeme>(token));
		}

		[[nodiscard]] std::unique_ptr<Parser::ILexeme> parseMuted(const std::vector<GrammarLexerTreeNode>& attributes)
		{
			const auto& lexeme = attributes.at(0);
			return std::make_unique<Parser::MutedLexeme>(parseLexeme(lexeme));
		}

		[[nodiscard]] std::unique_ptr<Parser::ILexeme> parseOptional(const std::vector<GrammarLexerTreeNode>& attributes)
		{
			const auto& lexeme = attributes.at(0);
			return std::make_unique<Parser::RecurrentLexeme>(Parser::Recurrence{parseLexeme(lexeme), Parser::RecurrenceType::Optional, 0});
		}

		[[nodiscard]] std::unique_ptr<Parser::ILexeme> parseRecurrence(const std::vector<GrammarLexerTreeNode>& attributes)
		{
			using namespace Type::Literals;
			const auto& token = std::get<Type::Token>(attributes.at(0).childs.at(0).value);
			const auto& recurrenceOperator = std::get<Type::Token>(attributes.at(1).value);
			const auto repetitions = std::get<unsigned int>(attributes.at(2).value);
			const auto& tokenExpression = m_languageLexer.getExpression(token);
			switch (recurrenceOperator.id)
			{
				case "equalTo"_token.id:
					return std::make_unique<Parser::RecurrentLexeme>(tokenExpression * repetitions);
				case "moreOrEqualTo"_token.id:
					return std::make_unique<Parser::RecurrentLexeme>(tokenExpression >= repetitions);
				case "moreThan"_token.id:
					return std::make_unique<Parser::RecurrentLexeme>(tokenExpression > repetitions);
				default:
					throw std::runtime_error{"Unknown operator type: " + std::string{recurrenceOperator.name}};
			}
		}

		[[nodiscard]] std::unique_ptr<Parser::ILexeme> parseAlternative(const std::vector<GrammarLexerTreeNode>& attributes)
		{
			auto lexemes = std::vector<std::unique_ptr<Parser::ILexeme>>{};
			lexemes.reserve(attributes.size());
			std::transform(attributes.begin(), attributes.end(), std::back_inserter(lexemes), [this](const auto& lexeme) -> std::unique_ptr<Parser::ILexeme> {
				return parseLexeme(lexeme);
			});
			return std::make_unique<Parser::AlternativeLexeme>(Parser::Alternative{std::move(lexemes)});
		}

		std::unordered_map<Type::Token, Parser::ParsingFunction<Types...>, Type::Token::hash_fn> m_parsingFunctions;
		Lexer<Type::Token, unsigned int> m_grammarLexer;
		Lexer<Types...> m_languageLexer;
	};
}
