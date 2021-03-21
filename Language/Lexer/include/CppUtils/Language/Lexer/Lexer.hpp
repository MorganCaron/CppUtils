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

		[[nodiscard]] inline Parser::Expression<Types...>& newExpression(const Type::Token& token, bool isNode = true)
		{
			if (!expressionExists(token))
				createExpression(token, isNode);
			return getExpression(token);
		}

		[[nodiscard]] inline Parser::Expression<Types...>& newExpression(const Type::Token& token, Type::Token name)
		{
			if (!expressionExists(token))
				createExpression(token, std::move(name));
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
			auto rootNode = Parser::ASTNode<Types...>{token};
			auto context = Parser::Context<Types...>{
				Parser::Cursor<std::string>{src, position},
				rootNode
			};
			try
			{
				parseSegment(token, context);
				if (!context.cursor.isEndOfString())
					throw std::runtime_error{"Syntax error: The string does not correspond to any known element."};
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
				throw std::runtime_error{"Syntax error in the \"" + std::string{token.name} + "\" expression."};
		}

	private:
		[[nodiscard]] inline bool parseExpression(const Parser::Expression<Types...>& expression, Parser::Context<Types...>& context) const
		{
			using namespace Type::Literals;

			if (expression.lexemes.empty())
				throw std::runtime_error{'"' + std::string{expression.token.name} + "\" expression is empty."};
			auto& [cursor, parentNode] = context;
			const auto startPosition = cursor.position;
			auto partialMatch = false;
			auto newParentNode = Parser::ASTNode<Types...>{parentNode.get().value};
			auto newContext = Parser::Context<Types...>{
				cursor,
				newParentNode
			};
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
			std::move(newParentNode.childs.begin(), newParentNode.childs.end(), std::back_inserter(parentNode.get().childs));
			return true;
		}

		inline bool parseNode(const Parser::Expression<Types...>& expression, Parser::Context<Types...>& context) const
		{
			auto& [cursor, parentNode] = context;
			if (!expression.name.isEmpty())
			{
				auto newNode = Parser::ASTNode<Types...>{expression.name};
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
			auto& [cursor, parentNode] = context;
			auto& parentChilds = parentNode.get().childs;
			auto tagChildId = parentChilds.size();
			const auto& tagLexeme = Type::ensureType<Parser::TagLexeme>(lexeme).value;
			if (!parseLexeme(tagLexeme, context) || parentChilds.size() <= tagChildId)
				return false;
			auto tagNode = std::move(parentChilds.at(tagChildId));
			parentChilds.erase(parentChilds.begin() + tagChildId);
			auto tagChilds = std::move(tagNode.childs);
			tagNode.childs = std::move(parentChilds);
			std::move(tagChilds.begin(), tagChilds.end(), std::back_inserter(tagNode.childs));
			parentChilds = std::vector<Parser::ASTNode<Types...>>{std::move(tagNode)};
			parentNode = parentChilds.at(0);
			return true;
		}

		[[nodiscard]] inline bool parseMutedLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			const auto& mutedLexeme = Type::ensureType<Parser::MutedLexeme>(lexeme).value;
			auto newNode = context.parentNode.get();
			auto newContext = Parser::Context<Types...>{
				context.cursor,
				newNode
			};
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
			auto& [cursor, parentNode] = context;
			const auto startPosition = cursor.position;
			for (const auto& lexeme : alternative.lexemes)
			{
				if (parseLexeme(lexeme, context))
					return true;
				cursor.position = startPosition;
			}
			return false;
		}

		std::unordered_map<Type::Token, Parser::Expression<Types...>, Type::Token::hash_fn> m_expressions;
		std::vector<std::vector<std::variant<Types...>>> m_lexemesCache;
	};
}
