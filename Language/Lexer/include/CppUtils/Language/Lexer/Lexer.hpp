#pragma once

#include <CppUtils/Language/Parser/Expression.hpp>
#include <CppUtils/String/String.hpp>

namespace CppUtils::Language::Lexer
{
	class Lexer final
	{
	private:
		struct Context final
		{
			Parser::Cursor<std::string> cursor;
			Graph::TokenNode parentNode;
		};

	public:
		[[nodiscard]] inline bool expressionExists(const Type::Token& token) const noexcept
		{
			return (m_expressions.find(token) != m_expressions.end());
		}

		[[nodiscard]] inline Parser::Expression& newExpression(const Type::Token& token, const bool isNode = true)
		{
			if (!expressionExists(token))
				m_expressions[token] = Parser::Expression{token, isNode};
			return m_expressions[token];
		}

		[[nodiscard]] inline const Parser::Expression& getExpression(const Type::Token& token) const
		{
			if (!expressionExists(token))
				throw std::runtime_error{"Undefined expression: " + std::string{token.name}};
			return m_expressions.at(token);
		}

		[[nodiscard]] Graph::TokenNode parse(const Type::Token& token, std::string_view src) const
		{
			const auto& expression = getExpression(token);
			auto position = std::size_t{0};
			auto context = Context{
				Parser::Cursor<std::string>{src, position},
				Graph::TokenNode{token}
			};

			if (!parseExpression(expression, context))
				throw std::runtime_error{"Syntax error in the " + std::string{token.name} + " expression."};
			if (!context.cursor.isEndOfString())
				throw std::runtime_error{"Syntax error:\nThe following string does not correspond to any known element:\n" + std::string{String::rightTrimString(context.cursor.getNextNChar(20))} + "..."};
			return context.parentNode;
		}

	private:
		[[nodiscard]] inline std::string getLexemeName(const std::unique_ptr<Parser::ILexeme>& lexeme) const
		{
			if (lexeme->getType() == Parser::StringLexemeType)
				return std::string{'"' + Type::ensureType<Parser::StringLexeme>(lexeme).value + '"'};
			if (lexeme->getType() == Parser::TokenLexemeType)
				return std::string{Type::ensureType<Parser::TokenLexeme>(lexeme).value.name};
			return "";
		}

		[[nodiscard]] inline bool parseExpression(const Parser::Expression& expression, Context& context) const
		{
			if (expression.lexemes.empty())
				throw std::runtime_error{"Undefined expression: " + std::string{expression.token.name}};
			const auto startPosition = context.cursor.position;
			auto partialMatch = false;
			for (const auto& lexeme : expression.lexemes)
			{
				if (!parseLexeme(lexeme, context))
				{
					if (partialMatch)
						throw std::runtime_error{"Syntax error in " + std::string{expression.token.name} + ":\n" + std::string{String::rightTrimString(context.cursor.getNextNChar(20))} + "...\nExpected format: " + std::string{lexeme->getType().name} + " " + getLexemeName(lexeme)};
					context.cursor.position = startPosition;
					return false;
				}
				if (lexeme->getType() == Parser::StringLexemeType)
					partialMatch = true;
			}
			return true;
		}

		inline bool parseNode(const Parser::Expression& expression, Context& context) const
		{
			auto& [cursor, parentNode] = context;

			if (expression.isNode)
			{
				auto newContext = Context{
					cursor,
					Graph::TokenNode{expression.token}
				};
				if (!parseExpression(expression, newContext))
					return false;
				parentNode.childs.emplace_back(newContext.parentNode);
			}
			else if (!parseExpression(expression, context))
				return false;
			return true;
		}

		[[nodiscard]] bool parseLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Context& context) const
		{
			switch (lexeme->getType().id)
			{
				case Parser::StringLexemeType.id:
					if (!parseStringLexeme(lexeme, context))
						return false;
					break;
				case Parser::ParserLexemeType.id:
					if (!parseParserLexeme(lexeme, context))
						return false;
					break;
				case Parser::TokenLexemeType.id:
					if (!parseTokenLexeme(lexeme, context))
						return false;
					break;
				case Parser::RecurrentLexemeType.id:
					if (!parseRecurrentLexeme(lexeme, context))
						return false;
					break;
				case Parser::AlternativeLexemeType.id:
					if (!parseAlternativeLexeme(lexeme, context))
						return false;
					break;
				default:
					throw std::runtime_error{"Unknown lexeme type: " + std::string{lexeme->getType().name}};
					break;
			}
			return true;
		}

		[[nodiscard]] inline bool parseStringLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Context& context) const
		{
			const auto& stringLexeme = Type::ensureType<Parser::StringLexeme>(lexeme);
			if (!context.cursor.isEqualSkipIt(stringLexeme.value))
				return false;
			return true;
		}

		[[nodiscard]] inline bool parseParserLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Context& context) const
		{
			auto& [cursor, parentNode] = context;
			const auto& parserLexeme = Type::ensureType<Parser::ParserLexeme>(lexeme);
			const auto startPosition = cursor.position;
			
			if (!parserLexeme.value(cursor, parentNode))
			{
				cursor.position = startPosition;
				return false;
			}
			return true;
		}

		[[nodiscard]] inline bool parseTokenLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Context& context) const
		{
			const auto& tokenLexeme = Type::ensureType<Parser::TokenLexeme>(lexeme);
			const auto& token = tokenLexeme.value;
			const auto& expression = getExpression(token);
			return parseNode(expression, context);
		}

		[[nodiscard]] inline bool parseRecurrentLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Context& context) const
		{
			const auto& recurrentLexeme = Type::ensureType<Parser::RecurrentLexeme>(lexeme);
			const auto& recurrence = recurrentLexeme.value;
			const auto& [token, recurrenceType, repetitions] = recurrence;
			const auto& expression = getExpression(token);
			
			switch (recurrenceType)
			{
				case Parser::RecurrenceType::Optional:
				{
					parseNode(expression, context);
					break;
				}
				case Parser::RecurrenceType::EqualTo:
				{
					for (auto i = repetitions; i > 0; --i)
						if (!parseNode(expression, context))
							return false;
					break;
				}
				case Parser::RecurrenceType::MoreThan:
				case Parser::RecurrenceType::MoreOrEqualTo:
				{
					auto i = 0u;
					while (parseNode(expression, context))
						++i;
					if ((recurrenceType == Parser::RecurrenceType::MoreThan && i <= repetitions) ||
						(recurrenceType == Parser::RecurrenceType::MoreOrEqualTo && i < repetitions))
						return false;
					break;
				}
				default:
					throw std::runtime_error{"Unknown recurrence type"};
			}
			return true;
		}

		[[nodiscard]] inline bool parseAlternativeLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Context& context) const
		{
			auto& [cursor, parentNode] = context;
			const auto& alternativeLexeme = Type::ensureType<Parser::AlternativeLexeme>(lexeme);
			const auto& alternative = alternativeLexeme.value;
			const auto startPosition = cursor.position;

			for (const auto& token : alternative.tokens)
			{
				const auto& expression = getExpression(token);
				if (parseNode(expression, context))
					return true;
				cursor.position = startPosition;
			}
			return false;
		}

		std::unordered_map<Type::Token, Parser::Expression, Type::Token::hash_fn> m_expressions;
	};
}
