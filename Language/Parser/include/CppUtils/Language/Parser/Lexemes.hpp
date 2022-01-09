#pragma once

#include <vector>
#include <memory>
#include <string>

#include <CppUtils/Type/Named.hpp>
#include <CppUtils/Type/Token.hpp>
#include <CppUtils/Type/Typed.hpp>
#include <CppUtils/Language/Parser/Context.hpp>

namespace CppUtils::Language::Parser
{
	using namespace Type::Literals;
	
	template<const Type::Token& StorageToken, typename StorageType>
	using Lexeme = Type::Typed<StorageToken, StorageType>;

	static constexpr auto CommaLexemeType = "comma"_token;
	using CommaLexeme = Lexeme<CommaLexemeType, void>;

	static constexpr auto BreakPointLexemeType = "breakpoint"_token;
	using BreakPointLexeme = Lexeme<BreakPointLexemeType, void>;
	
	static constexpr auto StringLexemeType = "string"_token;
	using StringLexeme = Lexeme<StringLexemeType, std::string>;

	template<typename... Types>
	struct NamedParser final: public Type::Named
	{
		NamedParser(std::string name, ParsingFunction<Types...> c_parsingFunction):
			Type::Named{std::move(name)},
			parsingFunction{std::move(c_parsingFunction)}
		{}

		ParsingFunction<Types...> parsingFunction;
	};

	template<typename... Types>
	NamedParser(std::string, ParsingFunction<Types...>) -> NamedParser<Types...>;

	template<typename... Types>
	NamedParser(std::string, bool(Context<Types...>&)) -> NamedParser<Types...>;

	static constexpr auto ParserLexemeType = "parser"_token;
	template<typename... Types>
	using ParserLexeme = Lexeme<ParserLexemeType, NamedParser<Types...>>; 

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

	enum class RecurrenceType: char
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

	std::ostream& operator<<(std::ostream& os, const Recurrence& recurrence)
	{
		os << '(';
		if (recurrence.type == RecurrenceType::Optional)
			os << '~';
		os << *recurrence.lexeme;
		switch (recurrence.type)
		{
			case RecurrenceType::EqualTo:
				os << " == ";
				break;
			case RecurrenceType::MoreThan:
				os << " > ";
				break;
			case RecurrenceType::MoreOrEqualTo:
				os << " >= ";
				break;
			default:
				break;
		}
		os << recurrence.repetitions << ')';
		return os;
	}

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

	std::ostream& operator<<(std::ostream& os, const Alternative& alternative)
	{
		const auto nbLexemes = alternative.lexemes.size();
		for (auto i = std::size_t{0}; i < nbLexemes; ++i)
		{
			const auto& lexeme = *alternative.lexemes[i];
			os << ((i == 0) ? "(" : " or ") << lexeme;
		}
		os << ')';
		return os;
	}

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
