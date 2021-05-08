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

		inline void createExpression(const Type::Token& token, bool isNode = true)
		{
			using namespace Type::Literals;
			m_expressions[token] = Parser::Expression<Types...>{token, isNode ? token : ""_token};
		}

		inline void createExpression(const Type::Token& token, Type::Token name)
		{
			m_expressions[token] = Parser::Expression<Types...>{token, std::move(name)};
		}

		template<typename... Args>
		[[nodiscard]] inline Parser::Expression<Types...>& newExpression(const Type::Token& token, Args&&... args)
		{
			if (!expressionExists(token))
				createExpression(token, std::forward<Args>(args)...);
			return getExpression(token);
		}

		[[nodiscard]] inline Parser::Expression<Types...>& getExpression(const Type::Token& token)
		{
			if (!expressionExists(token))
				throw std::runtime_error{"Undefined expression: " + std::string{token.name}};
			return m_expressions[token];
		}

		[[nodiscard]] inline const Parser::Expression<Types...>& getExpression(const Type::Token& token) const
		{
			if (!expressionExists(token))
				throw std::runtime_error{"Undefined expression: " + std::string{token.name}};
			return m_expressions.at(token);
		}

		[[nodiscard]] Parser::ASTNode<Types...> parseString(const Type::Token& token, std::string_view src) const
		{
			auto position = std::size_t{0};
			const auto& expression = getExpression(token);
			auto rootNode = Parser::ASTNode<Types...>{expression.name.isEmpty() ? token : expression.name};
			auto context = Parser::Context<Types...>{Parser::Cursor<std::string>{src, position}, rootNode, 0};
			try
			{
				if (!parseExpression(expression, context))
					throw std::runtime_error{"Syntax error in the \"" + std::string{token.name} + "\" expression."};
				if (!context.cursor.isEndOfString())
					throw std::runtime_error{"Syntax error: The string does not correspond to any known element."};
			}
			catch (const std::exception& exception)
			{
				throw std::runtime_error{"At line " + std::to_string(context.cursor.getLineNumber()) + ", position " + std::to_string(context.cursor.getPositionInTheLine())+ ":\n" + std::string{String::rightTrimString(context.cursor.getNextNChar(20))} + "...\n" + exception.what()};
			}
			return rootNode;
		}

		[[nodiscard]] inline bool parseSegment(const Type::Token& token, Parser::Context<Types...>& context) const
		{
			return parseNode(getExpression(token), context);
		}

	private:
		[[nodiscard]] inline bool parseExpression(const Parser::Expression<Types...>& expression, Parser::Context<Types...>& context) const
		{
			using namespace Type::Literals;
			auto& [cursor, parentNode, firstChildPosition] = context;
			if (expression.lexemes.empty())
				throw std::runtime_error{'"' + std::string{expression.token.name} + "\" expression is empty."};
			auto newParentNode = parentNode.get();
			auto newContext = Parser::Context<Types...>{cursor, newParentNode, firstChildPosition};
			auto partialMatch = false;
			const auto startPosition = cursor.position;
			for (const auto& lexeme : expression.lexemes)
			{
				if (!parseLexeme(lexeme, newContext))
				{
					if (partialMatch)
						throw std::runtime_error{"Syntax error in the \"" + std::string{expression.token.name} + "\" expression.\nExpected format: " + lexeme->getPrintable()};
					cursor.position = startPosition;
					return false;
				}
				partialMatch |= lexeme->getType() == Parser::StringLexemeType || lexeme->getType() == Parser::TagLexemeType;
			}
			parentNode.get() = newParentNode;
			return true;
		}

		inline bool parseNode(const Parser::Expression<Types...>& expression, Parser::Context<Types...>& context) const
		{
			auto& [cursor, parentNode, firstChildPosition] = context;
			if (expression.name.isEmpty())
			{
				auto& oldParentNode = parentNode.get();
				auto newContext = Parser::Context<Types...>{cursor, parentNode, oldParentNode.childs.size()};
				if (!parseExpression(expression, newContext))
					return false;
				parentNode = oldParentNode;
			}
			else
			{
				auto newNode = Parser::ASTNode<Types...>{expression.name};
				auto newContext = Parser::Context<Types...>{cursor, newNode, 0};
				if (!parseExpression(expression, newContext))
					return false;
				parentNode.get().childs.emplace_back(newNode);
			}
			return true;
		}

		bool parseLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
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
				case Parser::MutedLexemeType.id:
					return parseMutedLexeme(lexeme, context);
				case Parser::RecurrentLexemeType.id:
					return parseRecurrentLexeme(lexeme, context);
				case Parser::AlternativeLexemeType.id:
					return parseAlternativeLexeme(lexeme, context);
				case Parser::ExcludeLexemeType.id:
					return parseExcludeLexeme(lexeme, context);
				default:
					throw std::runtime_error{"Unknown lexeme type: " + std::string{lexeme->getType().name}};
			}
			return false;
		}

		[[nodiscard]] inline bool parseStringLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			const auto& string = Type::ensureType<Parser::StringLexeme>(lexeme).value;
			return context.cursor.isEqualSkipIt(string);
		}

		[[nodiscard]] inline bool parseParserLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			const auto& parser = Type::ensureType<Parser::ParserLexeme<Types...>>(lexeme).value;
			auto& [cursor, parentNode, firstChildPosition] = context;
			const auto startPosition = cursor.position;
			if (parser(context))
				return true;
			cursor.position = startPosition;
			return false;
		}

		[[nodiscard]] inline bool parseTokenLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			const auto& token = Type::ensureType<Parser::TokenLexeme>(lexeme).value;
			return parseSegment(token, context);
		}

		[[nodiscard]] inline bool parseTagLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			auto& [cursor, parentNode, firstChildPosition] = context;
			auto& parentChilds = parentNode.get().childs;
			auto tagPosition = parentChilds.size();
			const auto& tagLexeme = Type::ensureType<Parser::TagLexeme>(lexeme).value;
			if (!parseLexeme(tagLexeme, context) || parentChilds.size() <= tagPosition)
				return false;
			auto tagNode = std::move(parentChilds.at(tagPosition));
			auto newTagChilds = std::vector<Parser::ASTNode<Types...>>{};
			newTagChilds.reserve(parentChilds.size() - firstChildPosition - 1 + tagNode.childs.size());
			if (tagPosition > firstChildPosition)
				std::move(parentChilds.begin() + firstChildPosition, parentChilds.begin() + tagPosition, std::back_inserter(newTagChilds));
			std::move(tagNode.childs.begin(), tagNode.childs.end(), std::back_inserter(newTagChilds));
			if (parentChilds.size() > tagPosition)
				std::move(parentChilds.begin() + tagPosition + 1, parentChilds.end(), std::back_inserter(newTagChilds));
			parentChilds.erase(parentChilds.begin() + firstChildPosition, parentChilds.end());
			tagNode.childs = std::move(newTagChilds);
			parentNode = parentChilds.emplace_back(tagNode);
			return true;
		}

		[[nodiscard]] inline bool parseMutedLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			const auto& mutedLexeme = Type::ensureType<Parser::MutedLexeme>(lexeme).value;
			auto newNode = context.parentNode.get();
			auto newContext = Parser::Context<Types...>{context.cursor, newNode, 0};
			return parseLexeme(mutedLexeme, newContext);
		}

		[[nodiscard]] inline bool parseRecurrentLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			const auto& recurrence = Type::ensureType<Parser::RecurrentLexeme>(lexeme).value;
			const auto& [recurrenceLexeme, recurrenceType, repetitions] = recurrence;
			switch (recurrenceType)
			{
				case Parser::RecurrenceType::Optional:
				{
					parseLexeme(recurrenceLexeme, context);
					break;
				}
				case Parser::RecurrenceType::EqualTo:
				{
					for (auto i = repetitions; i > 0; --i)
						if (!parseLexeme(recurrenceLexeme, context))
							return false;
					break;
				}
				case Parser::RecurrenceType::MoreThan:
				case Parser::RecurrenceType::MoreOrEqualTo:
				{
					auto i = std::size_t{0};
					while (parseLexeme(recurrenceLexeme, context))
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
			auto& [cursor, parentNode, firstChildPosition] = context;
			const auto startPosition = cursor.position;
			for (const auto& alternativeLexeme : alternative.lexemes)
			{
				if (parseLexeme(alternativeLexeme, context))
					return true;
				cursor.position = startPosition;
			}
			return false;
		}

		[[nodiscard]] inline bool parseExcludeLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			const auto& exclusion = Type::ensureType<Parser::ExcludeLexeme>(lexeme).value;
			auto& [cursor, parentNode, firstChildPosition] = context;
			auto newParentNode = Parser::ASTNode<Types...>{parentNode.get().value};
			auto newContext = Parser::Context<Types...>{cursor, newParentNode, firstChildPosition};
			const auto startPosition = cursor.position;
			if (parseLexeme(exclusion.excluded, newContext))
			{
				cursor.position = startPosition;
				return false;
			}
			cursor.position = startPosition;
			if (parseLexeme(exclusion.lexeme, context))
				return true;
			cursor.position = startPosition;
			return false;
		}

		std::unordered_map<Type::Token, Parser::Expression<Types...>, Type::Token::hash_fn> m_expressions;
	};
}
