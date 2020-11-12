#pragma once

#include <vector>
#include <memory>
#include <string>
#include <string_view>

#include <Type/Typed.hpp>
#include <Type/TypeId.hpp>
#include <Graph/TreeNode.hpp>
#include <Language/Reader.hpp>

namespace CppUtils::Language::Lexeme
{
	using namespace Type::Literals;

	using Token = Type::TypeId;
	using TokenNode = Graph::TreeNode<Token>;

	using ILexeme = Type::ITyped;
	template<const Type::TypeId& StorageTypeId, typename StorageType>
	using Lexeme = Type::Typed<StorageTypeId, StorageType>;
	
	static constexpr auto StringLexemeType = "StringLexeme"_typeId;
	using StringLexeme = Lexeme<StringLexemeType, std::string>;

	static constexpr auto ParserLexemeType = "ParserLexeme"_typeId;
	using ParserFunction = std::function<bool (Reader::Cursor&, TokenNode&)>;
	using ParserLexeme = Lexeme<ParserLexemeType, ParserFunction>; 

	static constexpr auto TokenLexemeType = "TokenLexeme"_typeId;
	using TokenLexeme = Lexeme<TokenLexemeType, Token>;

	enum class RecurrenceType
	{
		Optional,
		EqualTo,
		MoreThan,
		MoreOrEqualTo,
	};

	struct Recurrence final
	{
		Token token;
		RecurrenceType type;
		std::size_t repetitions;
	};

	static constexpr auto RecurrentLexemeType = "RecurrentLexeme"_typeId;
	using RecurrentLexeme = Lexeme<RecurrentLexemeType, Recurrence>;

	struct Definition;
	struct Contingence final
	{
		std::vector<Token> tokens;

		[[nodiscard]] Contingence& operator||(const Definition& rhs);
	};

	static constexpr auto ContingentLexemeType = "ContingentLexeme"_typeId;
	using ContingentLexeme = Lexeme<ContingentLexemeType, Contingence>;

	struct Definition final
	{
		Token token;
		bool isNode;
		std::vector<std::unique_ptr<ILexeme>> lexemes;

		Definition() = default;
		explicit Definition(Token c_token, const bool c_isNode):
			token{std::move(c_token)},
			isNode{c_isNode}
		{};

		Definition& operator>>(std::string string)
		{
			lexemes.emplace_back(std::make_unique<StringLexeme>(std::move(string)));
			return *this;
		}

		Definition& operator>>(char c)
		{
			lexemes.emplace_back(std::make_unique<StringLexeme>(std::string{c}));
			return *this;
		}

		Definition& operator>>(const Definition& lexeme)
		{
			lexemes.emplace_back(std::make_unique<TokenLexeme>(lexeme.token));
			return *this;
		}

		Definition& operator>>(ParserFunction function)
		{
			lexemes.emplace_back(std::make_unique<ParserLexeme>(std::move(function)));
			return *this;
		}

		Definition& operator>>(Recurrence recurrence)
		{
			lexemes.emplace_back(std::make_unique<RecurrentLexeme>(std::move(recurrence)));
			return *this;
		}

		Definition& operator>>(Contingence contingence)
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

		[[nodiscard]] Contingence operator||(const Definition& rhs) const
		{
			return Contingence{std::vector<Token>{token, rhs.token}};
		}
	};
}
