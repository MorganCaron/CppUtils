#pragma once

#include <map>
#include <unordered_map>

#include <CppUtils/Language/Parser/Modifiers.hpp>
#include <CppUtils/Language/Lexer/GrammarLexer.hpp>

namespace CppUtils::Parameters
{
	class ParametersLexer final
	{
	public:
		ParametersLexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;

			m_grammarLexer.addParsingFunction("keywordParser"_token, Language::Parser::keywordParser<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("contentParser"_token, Language::Parser::escapeCharParser<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("trimModifier"_token, Language::Parser::Modifier::trimModifier<Type::Token, std::string>);

			static constexpr auto grammarSrc = R"(
			main: _spaces (command >= 0) _spaces;
			command: _spaces keywordParser ~_value;
			!_value: _spaces '[' ((contentParser != ']') >= 0) ']' ~trimModifier;
			!_spaces: (_space >= 0);
			!_space: (' ' || '\t' || '\n');
			)"sv;
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] inline std::map<std::string, std::string> parseParameters(const std::size_t argc, const char *argv[]) const
		{
			using namespace Type::Literals;
			const auto argumentVector = String::cstringArrayToVectorOfStrings<std::string_view>(argv + 1, argc - 1);
			const auto src = String::concatenateStringsWithDelimiter(argumentVector, " ");
			const auto commandTree = m_grammarLexer.parseLanguage("main"_token, src);
			auto parameters = std::map<std::string, std::string>{};
			for (const auto& command : commandTree.childs)
				parameters[std::string{std::get<Type::Token>(command.getChildValue(0)).name}] = (command.childs.size() == 2 ? std::get<std::string>(command.getChildValue(1)) : "");
			return parameters;
		}

		[[nodiscard]] inline bool executeCommands(const std::size_t argc, const char *argv[], const std::unordered_map<std::string_view, std::function<bool(std::string_view)>>& commands) const
		{
			const auto parameters = parseParameters(argc, argv);
			for (const auto& [parameterName, parameterValue] : parameters)
			{
				if (const auto& commandIt = commands.find(parameterName); commandIt != commands.end())
				{
					if (commandIt->second(parameterValue))
						return true;
				}
				else
					Log::Logger::logWarning("Unknown command: " + parameterName);
			}
			return false;
		}

	private:
		Language::Lexer::GrammarLexer<Type::Token, std::string> m_grammarLexer;
	};
}
