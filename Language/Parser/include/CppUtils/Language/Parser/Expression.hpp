#pragma once

#include <vector>
#include <memory>
#include <string>
#include <variant>
#include <string_view>

#include <CppUtils/Type/Typed.hpp>
#include <CppUtils/Type/Token.hpp>
#include <CppUtils/Graph/TreeNode.hpp>
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

	template<typename... Types> struct Expression;
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

	struct Alternative;
	template<typename... Types>
	struct Expression final
	{
		Type::Token token;
		Type::Token name;
		std::vector<std::unique_ptr<ILexeme>> lexemes;

		Expression() = default;
		explicit Expression(Type::Token c_token, Type::Token c_name, std::vector<std::unique_ptr<ILexeme>> c_lexemes = {}):
			token{std::move(c_token)},
			name{std::move(c_name)},
			lexemes{std::move(c_lexemes)}
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

		Expression& operator>>(const Expression& expression)
		{
			lexemes.emplace_back(std::make_unique<TokenLexeme>(expression.token));
			return *this;
		}
		
		Expression& operator>>(ParsingFunction<Types...> function)
		{
			lexemes.emplace_back(std::make_unique<ParserLexeme<Types...>>(std::move(function)));
			return *this;
		}

		Expression& operator>>(TagLexeme tagLexeme)
		{
			lexemes.emplace_back(std::make_unique<TagLexeme>(std::move(tagLexeme)));
			return *this;
		}

		Expression& operator>>(Recurrence recurrence)
		{
			lexemes.emplace_back(std::make_unique<RecurrentLexeme>(std::move(recurrence)));
			return *this;
		}

		Expression& operator>>(Alternative alternative)
		{
			lexemes.emplace_back(std::make_unique<AlternativeLexeme>(std::move(alternative)));
			return *this;
		}

		[[nodiscard]] Recurrence operator~() const
		{
			return Recurrence{std::make_unique<TokenLexeme>(token), RecurrenceType::Optional, 0};
		}

		[[nodiscard]] Recurrence operator>(std::size_t repetitions) const
		{
			return Recurrence{std::make_unique<TokenLexeme>(token), RecurrenceType::MoreThan, repetitions};
		}

		[[nodiscard]] Recurrence operator>=(std::size_t repetitions) const
		{
			return Recurrence{std::make_unique<TokenLexeme>(token), RecurrenceType::MoreOrEqualTo, repetitions};
		}

		[[nodiscard]] Recurrence operator*(std::size_t repetitions) const
		{
			return Recurrence{std::make_unique<TokenLexeme>(token), RecurrenceType::EqualTo, repetitions};
		}

		[[nodiscard]] Alternative operator||(const Expression& rhs) const
		{
			auto lexemes = std::vector<std::unique_ptr<ILexeme>>{};
			lexemes.emplace_back(std::make_unique<TokenLexeme>(token));
			lexemes.emplace_back(std::make_unique<TokenLexeme>(rhs.token));
			return Alternative{std::move(lexemes)};
		}
	};
}
