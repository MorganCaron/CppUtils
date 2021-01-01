#pragma once

#include <vector>
#include <memory>
#include <string>
#include <string_view>

#include <CppUtils/Type/Typed.hpp>
#include <CppUtils/Type/Token.hpp>
#include <CppUtils/Graph/TreeNode.hpp>
#include <CppUtils/Language/Parser/Cursor.hpp>

namespace CppUtils::Language::Parser
{
	using namespace Type::Literals;

	using ILexeme = Type::ITyped;
	template<const Type::Token& StorageToken, typename StorageType>
	using Lexeme = Type::Typed<StorageToken, StorageType>;
	
	static constexpr auto StringLexemeType = "string"_token;
	using StringLexeme = Lexeme<StringLexemeType, std::string>;

	static constexpr auto ParserLexemeType = "parser"_token;
	using ParserFunction = std::function<bool (Parser::Cursor&, Graph::TokenNode&)>;
	using ParserLexeme = Lexeme<ParserLexemeType, ParserFunction>; 

	static constexpr auto TokenLexemeType = "token"_token;
	using TokenLexeme = Lexeme<TokenLexemeType, Type::Token>;

	enum class RecurrenceType
	{
		Optional,
		EqualTo,
		MoreThan,
		MoreOrEqualTo
	};

	struct Recurrence final
	{
		Type::Token token;
		RecurrenceType type;
		std::size_t repetitions;
	};

	static constexpr auto RecurrentLexemeType = "recurrence"_token;
	using RecurrentLexeme = Lexeme<RecurrentLexemeType, Recurrence>;

	struct Expression;
	struct Contingence final
	{
		std::vector<Type::Token> tokens;

		[[nodiscard]] Contingence& operator||(const Expression& rhs);
	};

	static constexpr auto ContingentLexemeType = "contingency"_token;
	using ContingentLexeme = Lexeme<ContingentLexemeType, Contingence>;

	struct Expression final
	{
		Type::Token token;
		bool isNode;
		std::vector<std::unique_ptr<ILexeme>> lexemes;

		Expression() = default;
		explicit Expression(Type::Token c_token, const bool c_isNode):
			token{std::move(c_token)},
			isNode{c_isNode}
		{};

		Expression& operator>>(std::string string)
		{
			lexemes.emplace_back(std::make_unique<StringLexeme>(std::move(string)));
			return *this;
		}

		Expression& operator>>(char c)
		{
			lexemes.emplace_back(std::make_unique<StringLexeme>(std::string{c}));
			return *this;
		}

		Expression& operator>>(const Expression& lexeme)
		{
			lexemes.emplace_back(std::make_unique<TokenLexeme>(lexeme.token));
			return *this;
		}

		Expression& operator>>(ParserFunction function)
		{
			lexemes.emplace_back(std::make_unique<ParserLexeme>(std::move(function)));
			return *this;
		}

		Expression& operator>>(Recurrence recurrence)
		{
			lexemes.emplace_back(std::make_unique<RecurrentLexeme>(std::move(recurrence)));
			return *this;
		}

		Expression& operator>>(Contingence contingence)
		{
			lexemes.emplace_back(std::make_unique<ContingentLexeme>(std::move(contingence)));
			return *this;
		}

		[[nodiscard]] Recurrence operator!() const
		{
			return Recurrence{token, RecurrenceType::Optional, 0};
		}

		[[nodiscard]] Recurrence operator>(std::size_t repetitions) const
		{
			return Recurrence{token, RecurrenceType::MoreThan, repetitions};
		}

		[[nodiscard]] Recurrence operator>=(std::size_t repetitions) const
		{
			return Recurrence{token, RecurrenceType::MoreOrEqualTo, repetitions};
		}

		[[nodiscard]] Recurrence operator*(std::size_t repetitions) const
		{
			return Recurrence{token, RecurrenceType::EqualTo, repetitions};
		}

		[[nodiscard]] Contingence operator||(const Expression& rhs) const
		{
			return Contingence{std::vector<Type::Token>{token, rhs.token}};
		}
	};
}
