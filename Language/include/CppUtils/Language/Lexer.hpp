#pragma once

#include <CppUtils/Language/Lexeme.hpp>
#include <CppUtils/String/String.hpp>

namespace CppUtils::Language
{
	using namespace std::literals;
	using namespace Type::Literals;

	class Lexer final
	{
	private:
		struct Context final
		{
			Reader::Cursor cursor;
			Lexeme::TokenNode parentNode;
		};

	public:
		inline Lexeme::Definition& lexeme(const Lexeme::Token& token, const bool isNode = true)
		{
			if (m_lexemeDefinitions.find(token) == m_lexemeDefinitions.end())
				m_lexemeDefinitions[token] = Lexeme::Definition{token, isNode};
			return m_lexemeDefinitions[token];
		}

		inline const Lexeme::Definition& getDefinition(const Lexeme::Token& token) const
		{
			if (m_lexemeDefinitions.find(token) == m_lexemeDefinitions.end())
				throw std::runtime_error{"Undefined definition: " + std::string{token.name}};
			return m_lexemeDefinitions.at(token);
		}

		[[nodiscard]] Lexeme::TokenNode parse(const Lexeme::Token& token, std::string_view src) const
		{
			const auto& definition = getDefinition(token);
			auto pos = std::size_t{0};
			auto context = Context{
				Reader::Cursor{src, pos},
				Lexeme::TokenNode{token}
			};

			if (!parseDefinition(definition, context))
				throw std::runtime_error{"Syntax error in the " + std::string{token.name} + " definition."};
			if (!context.cursor.isEndOfString())
				throw std::runtime_error{"Syntax error:\nThe following string does not correspond to any known element:\n" + std::string{String::rightTrimString(context.cursor.getNextNChar(20))} + "..."};
			return context.parentNode;
		}

	private:
		[[nodiscard]] inline std::string getLexemeName(const std::unique_ptr<Lexeme::ILexeme>& lexeme) const
		{
			if (lexeme->getType() == Lexeme::StringLexemeType)
				return std::string{'"' + Type::ensureType<Lexeme::StringLexeme>(lexeme).value + '"'};
			if (lexeme->getType() == Lexeme::TokenLexemeType)
				return std::string{Type::ensureType<Lexeme::TokenLexeme>(lexeme).value.name};
			return ""s;
		}

		[[nodiscard]] inline bool parseDefinition(const Lexeme::Definition& definition, Context& context) const
		{
			if (definition.lexemes.empty())
				throw std::runtime_error{"Undefined definition: " + std::string{definition.token.name}};
			const auto startPos = context.cursor.pos;
			auto partialMatch = false;
			for (const auto& lexeme : definition.lexemes)
			{
				if (!parseLexeme(lexeme, context))
				{
					if (partialMatch)
						throw std::runtime_error{"Syntax error in " + std::string{definition.token.name} + ":\n" + std::string{String::rightTrimString(context.cursor.getNextNChar(20))} + "...\nExpected format: " + std::string{lexeme->getType().name} + " " + getLexemeName(lexeme)};
					context.cursor.pos = startPos;
					return false;
				}
				if (lexeme->getType() == Lexeme::StringLexemeType)
					partialMatch = true;
			}
			return true;
		}

		inline bool parseNode(const Lexeme::Definition& definition, Context& context) const
		{
			auto& [cursor, parentNode] = context;

			if (definition.isNode)
			{
				auto newContext = Context{
					cursor,
					Lexeme::TokenNode{definition.token}
				};
				if (!parseDefinition(definition, newContext))
					return false;
				parentNode.childs.emplace_back(newContext.parentNode);
			}
			else if (!parseDefinition(definition, context))
				return false;
			return true;
		}

		[[nodiscard]] bool parseLexeme(const std::unique_ptr<Lexeme::ILexeme>& lexeme, Context& context) const
		{
			switch (lexeme->getType().id)
			{
				case Lexeme::StringLexemeType.id:
					if (!parseStringLexeme(lexeme, context))
						return false;
					break;
				case Lexeme::ParserLexemeType.id:
					if (!parseParserLexeme(lexeme, context))
						return false;
					break;
				case Lexeme::TokenLexemeType.id:
					if (!parseTokenLexeme(lexeme, context))
						return false;
					break;
				case Lexeme::RecurrentLexemeType.id:
					if (!parseRecurrentLexeme(lexeme, context))
						return false;
					break;
				case Lexeme::ContingentLexemeType.id:
					if (!parseContingentLexeme(lexeme, context))
						return false;
					break;
				default:
					throw std::runtime_error{"Unknown lexeme type: " + std::string{lexeme->getType().name}};
					break;
			}
			return true;
		}

		[[nodiscard]] inline bool parseStringLexeme(const std::unique_ptr<Lexeme::ILexeme>& lexeme, Context& context) const
		{
			const auto& stringLexeme = Type::ensureType<Lexeme::StringLexeme>(lexeme);
			if (!context.cursor.isEqualSkipIt(stringLexeme.value))
				return false;
			return true;
		}

		[[nodiscard]] inline bool parseParserLexeme(const std::unique_ptr<Lexeme::ILexeme>& lexeme, Context& context) const
		{
			auto& [cursor, parentNode] = context;
			const auto& parserLexeme = Type::ensureType<Lexeme::ParserLexeme>(lexeme);
			const auto startPos = cursor.pos;
			
			if (!parserLexeme.value(cursor, parentNode))
			{
				cursor.pos = startPos;
				return false;
			}
			return true;
		}

		[[nodiscard]] inline bool parseTokenLexeme(const std::unique_ptr<Lexeme::ILexeme>& lexeme, Context& context) const
		{
			const auto& tokenLexeme = Type::ensureType<Lexeme::TokenLexeme>(lexeme);
			const auto& token = tokenLexeme.value;
			const auto& definition = getDefinition(token);
			return parseNode(definition, context);
		}

		[[nodiscard]] inline bool parseRecurrentLexeme(const std::unique_ptr<Lexeme::ILexeme>& lexeme, Context& context) const
		{
			const auto& recurrentLexeme = Type::ensureType<Lexeme::RecurrentLexeme>(lexeme);
			const auto& recurrence = recurrentLexeme.value;
			const auto& [token, recurrenceType, repetitions] = recurrence;
			const auto& definition = getDefinition(token);
			
			switch (recurrenceType)
			{
				case Lexeme::RecurrenceType::Optional:
				{
					parseNode(definition, context);
					break;
				}
				case Lexeme::RecurrenceType::EqualTo:
				{
					for (auto i = repetitions; i > 0; --i)
						if (!parseNode(definition, context))
							return false;
					break;
				}
				case Lexeme::RecurrenceType::MoreThan:
				case Lexeme::RecurrenceType::MoreOrEqualTo:
				{
					auto i = 0u;
					while (parseNode(definition, context))
						++i;
					if ((recurrenceType == Lexeme::RecurrenceType::MoreThan && i <= repetitions) ||
						(recurrenceType == Lexeme::RecurrenceType::MoreOrEqualTo && i < repetitions))
						return false;
					break;
				}
				default:
					throw std::runtime_error{"Unknown recurrence type"};
			}
			return true;
		}

		[[nodiscard]] inline bool parseContingentLexeme(const std::unique_ptr<Lexeme::ILexeme>& lexeme, Context& context) const
		{
			auto& [cursor, parentNode] = context;
			const auto& contingentLexeme = Type::ensureType<Lexeme::ContingentLexeme>(lexeme);
			const auto& contingence = contingentLexeme.value;
			const auto startPos = cursor.pos;

			for (const auto& token : contingence.tokens)
			{
				const auto& definition = getDefinition(token);
				if (parseNode(definition, context))
					return true;
				cursor.pos = startPos;
			}
			return false;
		}

		std::unordered_map<Lexeme::Token, Lexeme::Definition, Lexeme::Token::hash_fn> m_lexemeDefinitions;
	};
}
