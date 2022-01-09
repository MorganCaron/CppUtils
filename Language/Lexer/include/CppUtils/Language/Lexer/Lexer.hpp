#pragma once

#include <CppUtils/Language/Parser/Expression.hpp>
#include <CppUtils/String/String.hpp>
#include <CppUtils/Log/Logger.hpp>

namespace CppUtils::Language::Lexer
{
	template<typename... Types>
	class Lexer final
	{
	public:
		[[nodiscard]] inline bool expressionExists(const Type::Token& token) const noexcept
		{
			return m_expressions.find(token) != m_expressions.end();
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

		[[nodiscard]] inline bool parseSegment(const Type::Token& token, Parser::Context<Types...>& context) const
		{
			return parseNode(getExpression(token), context);
		}

		void parseContext(const Type::Token& token, Parser::Context<Types...>& context) const
		{
			using namespace std::literals;
			try
			{
				auto parsingSuccess = parseExpression(getExpression(token), context);
				auto details = buildErrorMessage(context);
				details = (details.empty() ? "" : "\n" + details);
				if (!parsingSuccess)
					throw std::runtime_error{"Syntax error in the \"" + std::string{token.name} + "\" expression." + details};
				if (!context.cursor.isEndOfString())
					throw std::runtime_error{"Syntax error: The following string does not match any known element."s + details};
			}
			catch (const std::exception& exception)
			{
				throw std::runtime_error{"In the parser/lexer:\nAt line " + std::to_string(context.cursor.getLineNumber()) + ", position " + std::to_string(context.cursor.getPositionInTheLine())+ ":\n" + std::string{String::rightTrimString(context.cursor.getNextNChar(20))} + "...\n" + exception.what()};
			}
		}

		[[nodiscard]] Parser::ASTNode<Types...> parseString(const Type::Token& token, std::string_view src) const
		{
			auto position = std::size_t{0};
			const auto& expression = getExpression(token);
			auto rootNode = Parser::ASTNode<Types...>{expression.name.isEmpty() ? token : expression.name};
			auto context = Parser::Context<Types...>{Parser::Cursor<std::string>{src, position}, rootNode, 0};
			parseContext(token, context);
			return rootNode;
		}

	private:
		[[nodiscard]] std::string buildErrorMessage(const Parser::Context<Types...>& context) const
		{
			using namespace std::literals;
			auto position = std::size_t{0};
			auto cursor = Parser::Cursor<std::string>{context.cursor.src, position};
			auto message = ""s;
			for (const auto& [cursorPosition, lexeme] : context.parsingErrors)
			{
				position = cursorPosition;
				if (!message.empty())
					message += "or\n";
				message += "At line " + std::to_string(cursor.getLineNumber()) + ", position " + std::to_string(cursor.getPositionInTheLine())+ ":\n" + std::string{String::rightTrimString(cursor.getNextNChar(20))} + "...\n";
				message += "Expected format: " + lexeme->getPrintable() + '\n';
			}
			return message;
		}
		
		[[nodiscard]] bool parseExpression(const Parser::Expression<Types...>& expression, Parser::Context<Types...>& context) const
		{
			using namespace std::literals;
			auto& [cursor, parentNode, firstChildPosition, parsingErrors] = context;
			if (expression.lexemes.empty())
				throw std::runtime_error{'"' + std::string{expression.token.name} + "\" expression is empty."};
			auto newParentNode = parentNode.get();
			auto newContext = Parser::Context<Types...>{cursor, newParentNode, firstChildPosition};
			auto partialMatch = false;
			const auto startPosition = cursor.position;
			const auto nbLexemes = expression.lexemes.size();
			for (auto i = std::size_t{0}; i < nbLexemes; ++i)
			{
				const auto& lexeme = expression.lexemes[i];
				if (lexeme->getType() == Parser::CommaLexemeType)
				{
					partialMatch = true;
					parsingErrors.clear();
					continue;
				}
				auto parsingSuccess = parseLexeme(lexeme, newContext);
				Container::Vector::merge(parsingErrors, newContext.parsingErrors);
				if (!parsingSuccess)
				{
					cursor.position = startPosition;
					if (partialMatch)
						throw std::runtime_error{"Syntax error in the \"" + std::string{expression.token.name} + "\" expression.\nExpected format: " + lexeme->getPrintable()};
					return false;
				}
				partialMatch |= lexeme->getType() == Parser::StringLexemeType || lexeme->getType() == Parser::TagLexemeType;
			}
			parentNode.get() = newParentNode;
			return true;
		}

		bool parseNode(const Parser::Expression<Types...>& expression, Parser::Context<Types...>& context) const
		{
			auto& [cursor, parentNode, firstChildPosition, parsingErrors] = context;
			if (expression.name.isEmpty())
			{
				auto& oldParentNode = parentNode.get();
				auto newContext = Parser::Context<Types...>{cursor, parentNode, oldParentNode.childs.size()};
				auto parsingSuccess = parseExpression(expression, newContext);
				Container::Vector::merge(parsingErrors, newContext.parsingErrors);
				if (!parsingSuccess)
					return false;
				parentNode = oldParentNode;
			}
			else
			{
				auto newNode = Parser::ASTNode<Types...>{expression.name};
				auto newContext = Parser::Context<Types...>{cursor, newNode, 0};
				auto parsingSuccess = parseExpression(expression, newContext);
				Container::Vector::merge(parsingErrors, newContext.parsingErrors);
				if (!parsingSuccess)
					return false;
				parentNode.get().childs.emplace_back(newNode);
			}
			return true;
		}

		bool parseLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			switch (lexeme->getType().id)
			{
				case Parser::BreakPointLexemeType.id:
					return parseBreakPointLexeme(context);
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

		[[nodiscard]] inline bool parseBreakPointLexeme(const Parser::Context<Types...>& context) const
		{
			using namespace std::literals;
			const auto& cursor = context.cursor;
			Log::Logger::logDebug("Breakpoint:\n"s + "At line " + std::to_string(cursor.getLineNumber()) + ", position " + std::to_string(cursor.getPositionInTheLine())+ ":\n" + std::string{String::rightTrimString(cursor.getNextNChar(20))} + "...");
			return true;
		}

		[[nodiscard]] inline bool parseStringLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			const auto& string = Type::ensureType<Parser::StringLexeme>(lexeme).value;
			auto& [cursor, parentNode, firstChildPosition, parsingErrors] = context;
			if (!cursor.isEqualSkipIt(string))
			{
				parsingErrors.push_back(Parser::ParsingError{
					.cursorPosition = cursor.position,
					.lexeme = lexeme.get()
				});
				return false;
			}
			return true;
		}

		[[nodiscard]] inline bool parseParserLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			const auto& namedParser = Type::ensureType<Parser::ParserLexeme<Types...>>(lexeme).value;
			auto& [cursor, parentNode, firstChildPosition, parsingErrors] = context;
			const auto startPosition = cursor.position;
			if (namedParser.parsingFunction(context))
				return true;
			cursor.position = startPosition;
			parsingErrors.push_back(Parser::ParsingError{
				.cursorPosition = startPosition,
				.lexeme = lexeme.get()
			});
			return false;
		}

		[[nodiscard]] inline bool parseTokenLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			const auto& token = Type::ensureType<Parser::TokenLexeme>(lexeme).value;
			return parseSegment(token, context);
		}

		[[nodiscard]] inline bool parseTagLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			auto& [cursor, parentNode, firstChildPosition, parsingErrors] = context;
			auto& parentChilds = parentNode.get().childs;
			auto tagPosition = parentChilds.size();
			const auto& tagLexeme = Type::ensureType<Parser::TagLexeme>(lexeme).value;
			if (!parseLexeme(tagLexeme, context) || parentChilds.size() <= tagPosition)
			{
				parsingErrors.push_back(Parser::ParsingError{
					.cursorPosition = cursor.position,
					.lexeme = lexeme.get()
				});
				return false;
			}
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
			if (!parseLexeme(mutedLexeme, newContext))
			{
				Container::Vector::merge(context.parsingErrors, newContext.parsingErrors);
				return false;
			}
			Container::Vector::merge(context.parsingErrors, newContext.parsingErrors);
			return true;
		}

		[[nodiscard]] inline bool parseRecurrentLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			const auto& recurrence = Type::ensureType<Parser::RecurrentLexeme>(lexeme).value;
			const auto& [recurrenceLexeme, recurrenceType, repetitions] = recurrence;
			auto& [cursor, parentNode, firstChildPosition, parsingErrors] = context;
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
					{
						parsingErrors.push_back(Parser::ParsingError{
							.cursorPosition = cursor.position,
							.lexeme = recurrenceLexeme.get()
						});
						return false;
					}
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
			auto& [cursor, parentNode, firstChildPosition, parsingErrors] = context;
			const auto startPosition = cursor.position;
			const auto nbParsingErrors = parsingErrors.size();
			for (const auto& alternativeLexeme : alternative.lexemes)
			{
				if (parseLexeme(alternativeLexeme, context))
					return true;
				cursor.position = startPosition;
			}
			parsingErrors.erase(parsingErrors.begin() + nbParsingErrors, parsingErrors.end());
			parsingErrors.push_back(Parser::ParsingError{
				.cursorPosition = startPosition,
				.lexeme = lexeme.get()
			});
			return false;
		}

		[[nodiscard]] inline bool parseExcludeLexeme(const std::unique_ptr<Parser::ILexeme>& lexeme, Parser::Context<Types...>& context) const
		{
			const auto& exclusion = Type::ensureType<Parser::ExcludeLexeme>(lexeme).value;
			auto& [cursor, parentNode, firstChildPosition, parsingErrors] = context;
			auto newParentNode = Parser::ASTNode<Types...>{parentNode.get().value};
			auto newContext = Parser::Context<Types...>{cursor, newParentNode, firstChildPosition};
			const auto startPosition = cursor.position;
			const auto excludedLexemeFound = parseLexeme(exclusion.excluded, newContext);
			cursor.position = startPosition;
			if (excludedLexemeFound)
			{
				Container::Vector::merge(parsingErrors, newContext.parsingErrors);
				return false;
			}
			if (parseLexeme(exclusion.lexeme, context))
				return true;
			cursor.position = startPosition;
			parsingErrors.push_back(Parser::ParsingError{
				.cursorPosition = startPosition,
				.lexeme = lexeme.get()
			});
			return false;
		}

		std::unordered_map<Type::Token, Parser::Expression<Types...>, Type::Token::hash_fn> m_expressions;
	};
}
