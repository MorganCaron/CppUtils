#pragma once

#include <vector>
#include <memory>
#include <string>
#include <variant>
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
	template<typename... Types>
	using ParserFunction = std::function<bool (Parser::Cursor<std::string>&, Graph::VariantTreeNode<Types...>&)>;
	template<typename... Types>
	using ParserLexeme = Lexeme<ParserLexemeType, ParserFunction<Types...>>; 

	static constexpr auto TokenLexemeType = "token"_token;
	using TokenLexeme = Lexeme<TokenLexemeType, Type::Token>;

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
		Type::Token token;
		RecurrenceType type;
		std::size_t repetitions;
	};

	static constexpr auto RecurrentLexemeType = "recurrence"_token;
	using RecurrentLexeme = Lexeme<RecurrentLexemeType, Recurrence>;

	struct Alternative final
	{
		std::vector<Type::Token> tokens;

		template<typename... Types>
		[[nodiscard]] Alternative& operator||(const Expression<Types...>& rhs)
		{
			tokens.emplace_back(rhs.token);
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
		
		Expression& operator>>(ParserFunction<Types...> function)
		{
			lexemes.emplace_back(std::make_unique<ParserLexeme<Types...>>(std::move(function)));
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

		[[nodiscard]] Alternative operator||(const Expression& rhs) const
		{
			return Alternative{std::vector<Type::Token>{token, rhs.token}};
		}
	};
}
