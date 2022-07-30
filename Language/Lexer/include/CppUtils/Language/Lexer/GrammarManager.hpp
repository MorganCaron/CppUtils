#pragma once

#include <CppUtils/Hash/Token.hpp>

namespace CppUtils::Language::Lexer
{
	class GrammarManager final
	{
	public:
		GrammarManager()
		{
			using namespace std::literals;
			using namespace Hash::Literals;
			auto lowLevelGrammarAst = Parser::parseAst(R"(
				main_{ token_{decls_} optional_{token_{spaces_}} }
				decls_{ token_{decl_} optional_{token_{decls_}} }
				decl_{ optional_{token_{spaces_}} token_{token_} optional_{token_{spaces_}} , optional_{token_{name_}} , string_{:} , token_{lexemes_} optional_{token_{spaces_}} string_{;} }
				lexemes_{ optional_{token_{spaces_}} token_{lexeme_} optional_{token_{lexemes_}} }
				lexeme_{ token_{quote_} } lexeme_{ token_{comma_} } lexeme_{ token_{hash_} } lexeme_{ token_{underscore_} } lexeme_{ token_{optional_} } lexeme_{ token_{name_} } lexeme_{ token_{token_} }
				name_{ optional_{token_{spaces_}} string_{[} , token_{token_} optional_{token_{spaces_}} string_{]} }
				quote_{ token_{quote1_} } quote_{ token_{quote2_} }
				quote1_{ string_{'} , optional_{token_{nonquote1_}} string_{'} } quote2_{ string_{"} , optional_{token_{nonquote2_}} string_{"} }
				nonquote1_{ not_{string_{'}} add_{c}+ optional_{token_{nonquote1_}} } nonquote2_{ not_{string_{"}} add_{c}+ optional_{token_{nonquote2_}} }
				comma_{ string_{,} }
				underscore_{ string_{\_} hash_ }
				optional_{ string_{~} , token_{lexeme_} }
				token_{ token_{keyword_} optional_{token_{hash_}} }
				keyword_{ token_{char_} optional_{token_{keyword_}} }
				char_{ >=_{a} <=_{z} add_{c}+ } char_{ >=_{A} <=_{Z} add_{c}+ }
				spaces_{ token_{space_} optional_{token_{spaces_}} }
				space_{ string_{\ } } space_{ string_{\n} } space_{ string_{\t} } space_{ string_{\r} }
			)"sv);
			addGrammar("highLevelGrammar"_token, parse(R"(
				main: decls ~spaces;
				decls: decl ~decls;
				decl: ~spaces [token] ~spaces , ':' , lexemes ~spaces ';';
				lexemes: ~spaces lexeme ~lexemes;
				lexeme: keyword;
				token[t]: keyword '_';
				keyword: "main";
				spaces: space ~spaces;
				spaces: ' '; spaces: '\n'; spaces: '\t'; spaces: '\r';
			)"sv, lowLevelGrammarAst));
		}

		[[nodiscard]] auto getGrammar(Hash::Token token) const -> const Parser::Ast&
		{
			if (m_grammars.contains(token))
				return m_grammars.at(token);
			throw std::runtime_error{"Grammar not found"};
		}

		[[nodiscard]] auto parseGrammar(std::string_view src) const -> Parser::Ast
		{
			using namespace Hash::Literals;
			return parse(src, getGrammar("highLevelGrammar"_token));
		}

		[[nodiscard]] auto parseLanguage(std::string_view src, std::string_view grammarName) const -> Parser::Ast
		{
			using namespace std::literals;
			try
			{
				return parse(src, getGrammar(Hash::hash(grammarName)));
			}
			catch (const std::exception& exception)
			{
				std::throw_with_nested(std::runtime_error{"In the code in "s + std::data(grammarName) + " language"});
			}
		}

		auto addGrammar(Hash::Token token, Parser::Ast grammar) -> void
		{
			grammar.log();
			m_grammars[token] = std::move(grammar);
		}

		auto addGrammar(std::string_view grammarName, std::string_view grammarSource) -> void
		{
			using namespace std::literals;
			try
			{
				m_grammars[Hash::hash(grammarName)] = parseGrammar(grammarSource);
			}
			catch (const std::exception& exception)
			{
				std::throw_with_nested(std::runtime_error{"In "s + std::data(grammarName)});
			}
		}

	private:
		std::unordered_map<Hash::Token, Parser::Ast> m_grammars;
	};
}
