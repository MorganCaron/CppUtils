#pragma once

#include <vector>
#include <memory>
#include <string>

#include <CppUtils/Type/Token.hpp>
#include <CppUtils/Type/Typed.hpp>
#include <CppUtils/Language/Parser/Context.hpp>

namespace CppUtils::Language::Parser
{
	using namespace Type::Literals;

	using ILexeme = Type::ITyped;
	template<const Type::Token& StorageToken, typename StorageType>
	using Lexeme = Type::Typed<StorageToken, StorageType>;
	
	static constexpr auto StringLexemeType = "string"_token;
	using StringLexeme = Lexeme<StringLexemeType, std::string>;

	static constexpr auto ParserLexemeType = "parser"_token;
	template<typename... Types>
	using ParserLexeme = Lexeme<ParserLexemeType, ParsingFunction<Types...>>; 

	static constexpr auto TokenLexemeType = "token"_token;
	using TokenLexeme = Lexeme<TokenLexemeType, Type::Token>;

	static constexpr auto TagLexemeType = "tag"_token;
	using TagLexeme = Lexeme<TagLexemeType, std::unique_ptr<ILexeme>>;

	static constexpr auto MutedLexemeType = "muted"_token;
	using MutedLexeme = Lexeme<MutedLexemeType, std::unique_ptr<ILexeme>>;

	template<typename... Types>
	struct Expression;
	static constexpr auto ExpressionLexemeType = "expression"_token;
	template<typename... Types>
	using ExpressionLexeme = Lexeme<ExpressionLexemeType, Expression<Types...>>;

	enum class RecurrenceType
	{
		Optional,
		EqualTo,
		MoreThan,
		MoreOrEqualTo
	};

	struct Recurrence final
	{
		std::unique_ptr<ILexeme> lexeme;
		RecurrenceType type;
		std::size_t repetitions;
	};

	static constexpr auto RecurrentLexemeType = "recurrence"_token;
	using RecurrentLexeme = Lexeme<RecurrentLexemeType, Recurrence>;

	struct Alternative final
	{
		std::vector<std::unique_ptr<ILexeme>> lexemes;

		explicit Alternative(std::vector<std::unique_ptr<ILexeme>> c_lexemes):
			lexemes(std::move(c_lexemes))
		{}

		template<typename... Types>
		[[nodiscard]] Alternative& operator||(const Expression<Types...>& rhs)
		{
			lexemes.emplace_back(std::make_unique<TokenLexeme>(rhs.token));
			return *this;
		}
	};

	static constexpr auto AlternativeLexemeType = "alternative"_token;
	using AlternativeLexeme = Lexeme<AlternativeLexemeType, Alternative>;

	struct Exclusion final
	{
		std::unique_ptr<ILexeme> lexeme;
		std::unique_ptr<ILexeme> excluded;
	};

	static constexpr auto ExcludeLexemeType = "exclusion"_token;
	using ExcludeLexeme = Lexeme<ExcludeLexemeType, Exclusion>;
}
