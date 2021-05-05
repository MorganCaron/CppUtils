#pragma once

#include <variant>
#include <string_view>

#include <CppUtils/Language/Parser/Lexemes.hpp>

namespace CppUtils::Language::Parser
{
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
			auto mergedLexemes = std::vector<std::unique_ptr<ILexeme>>{};
			mergedLexemes.emplace_back(std::make_unique<TokenLexeme>(token));
			mergedLexemes.emplace_back(std::make_unique<TokenLexeme>(rhs.token));
			return Alternative{std::move(mergedLexemes)};
		}

		[[nodiscard]] Exclusion operator!=(std::string string) const
		{
			return Exclusion{
				std::make_unique<TokenLexeme>(token),
				std::make_unique<StringLexeme>(std::move(string))};
		}

		[[nodiscard]] Exclusion operator!=(char c) const
		{
			return Exclusion{
				std::make_unique<TokenLexeme>(token),
				std::make_unique<StringLexeme>(std::string{c})};
		}

		[[nodiscard]] Exclusion operator!=(const Expression& expression) const
		{
			return Exclusion{
				std::make_unique<TokenLexeme>(token),
				std::make_unique<TokenLexeme>(expression.token)};
		}

		[[nodiscard]] Exclusion operator!=(ParsingFunction<Types...> function) const
		{
			return Exclusion{
				std::make_unique<TokenLexeme>(token),
				std::make_unique<ParserLexeme<Types...>>(std::move(function))};
		}

		[[nodiscard]] Exclusion operator!=(TagLexeme tagLexeme) const
		{
			return Exclusion{
				std::make_unique<TokenLexeme>(token),
				std::make_unique<TagLexeme>(std::move(tagLexeme))};
		}

		[[nodiscard]] Exclusion operator!=(Recurrence recurrence) const
		{
			return Exclusion{
				std::make_unique<TokenLexeme>(token),
				std::make_unique<RecurrentLexeme>(std::move(recurrence))};
		}

		[[nodiscard]] Exclusion operator!=(Alternative alternative) const
		{
			return Exclusion{
				std::make_unique<TokenLexeme>(token),
				std::make_unique<AlternativeLexeme>(std::move(alternative))};
		}
	};
}
