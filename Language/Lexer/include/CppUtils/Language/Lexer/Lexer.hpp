#pragma once

#include <CppUtils/Language/Parser/Expression.hpp>
#include <CppUtils/String/String.hpp>

namespace CppUtils::Language::Lexer
{
	template<typename... Types>
	class Lexer final
	{
	public:
		[[nodiscard]] inline bool expressionExists(const Type::Token& token) const noexcept
		{
			return (m_expressions.find(token) != m_expressions.end());
		}

		[[nodiscard]] inline Parser::Expression<Types...>& newExpression(const Type::Token& token, const bool isNode = true)
		{
			if (!expressionExists(token))
				m_expressions[token] = Parser::Expression<Types...>{token, isNode};
			return m_expressions[token];
		}

		[[nodiscard]] inline const Parser::Expression<Types...>& getExpression(const Type::Token& token) const
		{
			if (!expressionExists(token))
				throw std::runtime_error{"Undefined expression: " + std::string{token.name}};
			return m_expressions.at(token);
		}

		[[nodiscard]] Graph::VariantTreeNode<Types...> parseString(const Type::Token& token, std::string_view src) const
		{
			auto position = std::size_t{0};
			auto rootNode = Graph::VariantTreeNode<Types...>{token};
			auto context = Parser::Context<Types...>{
				Parser::Cursor<std::string>{src, position},
				rootNode
			};
			try
			{
				parseSegment(token, context);
				if (!context.cursor.isEndOfString())
					throw std::runtime_error{"Syntax error: The string does not correspond to any known element"};
			}
			catch (const std::exception& exception)
			{
				throw std::runtime_error{"At line " + std::to_string(context.cursor.getLineNumber()) + ", position " + std::to_string(context.cursor.getPositionInTheLine())+ ":\n" + std::string{String::rightTrimString(context.cursor.getNextNChar(20))} + "...\n" + exception.what()};
			}
			return rootNode;
		}

		void parseSegment(const Type::Token& token, Parser::Context<Types...>& context) const
		{
			const auto& expression = getExpression(token);
			if (!parseExpression(expression, context))
				throw std::runtime_error{"Syntax error in the " + std::string{token.name} + " expression"};
		}

	private:
		[[nodiscard]] inline bool parseExpression(const Parser::Expression<Types...>& expression, Parser::Context<Types...>& context) const
		{
			if (expression.lexemes.empty())
				throw std::runtime_error{std::string{expression.token.name} + " expression is empty"};
			auto& [cursor, parentNode] = context;
			const auto startPosition = cursor.position;
			auto partialMatch = false;
			for (const auto& lexeme : expression.lexemes)
			{
				if (!parseLexeme(lexeme, context))
				{
					if (partialMatch)
						throw std::runtime_error{"Syntax error in the " + std::string{expression.token.name} + " expression. Expected format: " + lexeme->getPrintable()};
					cursor.position = startPosition;
					return false;
				}
				partialMatch |= lexeme->getType() == Parser::StringLexemeType || lexeme->getType() == Parser::TagLexemeType;
			}
			return true;
		}

		inline bool parseNode(const Parser::Expression<Types...>& expression, Parser::Context<Types...>& context) const
		{
			auto& [cursor, parentNode] = context;
			if (expression.isNode)
			{
				auto newNode = Graph::VariantTreeNode<Types...>{expression.token};
				auto newContext = Parser::Context<Types...>{
					cursor,
					newNode
				};
				if (!parseExpression(expression, newContext))
					return false;
				parentNode.get().childs.emplace_back(newNode);
			}
			else
			{
				auto& oldParentNode = parentNode.get();
				if (!parseExpression(expression, context))
					return false;
				parentNode = oldParentNode;
			}
			return true;
		}

		[[nodiscard]] bool parseLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			switch (lexeme->getType().id)
			{
				case Parser::StringLexemeType.id:
					return parseStringLexeme(lexeme, context);
				case Parser::ParserLexemeType.id:
					return parseParserLexeme(lexeme, context);
				case Parser::TokenLexemeType.id:
					return parseTokenLexeme(lexeme, context);
				case Parser::TagLexemeType.id:
					return parseTagLexeme(lexeme, context);
				case Parser::RecurrentLexemeType.id:
					return parseRecurrentLexeme(lexeme, context);
				case Parser::AlternativeLexemeType.id:
					return parseAlternativeLexeme(lexeme, context);
				default:
					throw std::runtime_error{"Unknown lexeme type: " + std::string{lexeme->getType().name}};
			}
			return true;
		}

		[[nodiscard]] inline bool parseStringLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			const auto& string = Type::ensureType<Parser::StringLexeme>(lexeme).value;
			return context.cursor.isEqualSkipIt(string);
		}

		[[nodiscard]] inline bool parseParserLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			const auto& parser = Type::ensureType<Parser::ParserLexeme<Types...>>(lexeme).value;
			auto& [cursor, parentNode] = context;
			const auto startPosition = cursor.position;
			if (parser(context))
				return true;
			cursor.position = startPosition;
			return false;
		}

		[[nodiscard]] inline bool parseTokenLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			const auto& token = Type::ensureType<Parser::TokenLexeme>(lexeme).value;
			const auto& expression = getExpression(token);
			return parseNode(expression, context);
		}

		[[nodiscard]] inline bool parseTagLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			const auto& nameParser = Type::ensureType<Parser::TagLexeme<Types...>>(lexeme).value;
			auto& [cursor, parentNode] = context;
			const auto startPosition = cursor.position;
			if (!nameParser(context))
			{
				cursor.position = startPosition;
				return false;
			}
			auto nbChilds = parentNode.get().childs.size();
			if (nbChilds == 0)
				throw std::runtime_error{"The parsing function of the name lexeme must define a name"};
			parentNode = parentNode.get().childs.at(nbChilds - 1);
			return true;
		}

		[[nodiscard]] inline bool parseRecurrentLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			const auto& recurrence = Type::ensureType<Parser::RecurrentLexeme>(lexeme).value;
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
					auto i = std::size_t{0};
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

		[[nodiscard]] inline bool parseAlternativeLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			const auto& alternative = Type::ensureType<Parser::AlternativeLexeme>(lexeme).value;
			auto& [cursor, parentNode] = context;
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

		std::unordered_map<Type::Token, Parser::Expression<Types...>, Type::Token::hash_fn> m_expressions;
	};
}
