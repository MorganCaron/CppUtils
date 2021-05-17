#pragma once

#include <map>

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>

namespace CppUtils::Parameters
{
	struct Command final
	{
		std::string_view name;
		std::function<bool(std::string_view)> function;
	};

	class ParametersLexer final
	{
	public:
		ParametersLexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;

			m_grammarLexer.addParsingFunction("spaceParser"_token, Language::Parser::spaceParser<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("keywordParser"_token, Language::Parser::keywordParser<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("contentParser"_token, Language::Parser::escapeCharParser<Type::Token, std::string>);
			m_grammarLexer.addParsingFunction("trimModifier"_token, Language::Parser::Modifier::trimModifier<Type::Token, std::string>);

			static constexpr auto grammarSrc = R"(
			main: (command >= 0) spaceParser;
			command: spaceParser keywordParser ~value;
			!value: spaceParser '[' ((contentParser != ']') >= 0) ']' ~trimModifier;
			)"sv;
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] inline std::map<std::string, std::string> parseParameters(const std::size_t argc, const char *argv[]) const
		{
			using namespace Type::Literals;
			const auto parameters = String::cstringArrayToVectorOfStrings<std::string_view>(argv + 1, argc - 1);
			const auto src = String::concatenateStringsWithDelimiter(parameters, " ");
			const auto commandTree = m_grammarLexer.parseLanguage("main"_token, src);
			auto map = std::map<std::string, std::string>{};
			for (const auto& command : commandTree.childs)
				map[std::string{std::get<Type::Token>(command.childs.at(0).value).name}] = (command.childs.size() == 2 ? std::get<std::string>(command.childs.at(1).value) : "");
			return map;
		}

		[[nodiscard]] inline bool executeCommands(const std::size_t argc, const char *argv[], const std::vector<Command>& commands) const
		{
			const auto parameters = parseParameters(argc, argv);
			for (const auto& [parameterName, parameterValue] : parameters)
			{
				const auto& commandIt = std::find_if(commands.begin(), commands.end(), [&parameterName = parameterName](const Command& command) -> bool {
					return parameterName == command.name;
				});
				if (commandIt == commands.end())
					Log::Logger::logWarning("Unknown command: " + parameterName);
				else if (commandIt->function(parameterValue))
					return true;
			}
			return false;
		}

	private:
		Language::Lexer::GrammarLexer<Type::Token, std::string> m_grammarLexer;
	};
}
