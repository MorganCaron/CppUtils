#pragma once

#include <CppUtils/Hash/Token.hpp>
#include <CppUtils/Language/Lexer/Grammar.hpp>

namespace CppUtils::Language::Lexer::Grammar
{
	class GrammarManager final
	{
	public:
		GrammarManager()
		{
			using namespace std::literals;
			using namespace Hash::Literals;

			addGrammar("lowLevelGrammar"_token, Parser::parseAst(lowLevelGrammar));
			// getGrammar("lowLevelGrammar"_token).log();
			
			try
			{
				addGrammar("highLevelGrammar"_token, parse(highLevelGrammar, getGrammar("lowLevelGrammar"_token)));
				// getGrammar("highLevelGrammar"_token).log();
			}
			catch (const std::exception& exception)
			{
				std::throw_with_nested(std::runtime_error{"In highLevelGrammar:"});
			}
		}

		[[nodiscard]] auto getGrammar(Hash::Token token) const -> const Parser::Ast&
		{
			if (!m_grammars.contains(token))
				throw std::runtime_error{"Grammar not found"};
			return m_grammars.at(token);
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
