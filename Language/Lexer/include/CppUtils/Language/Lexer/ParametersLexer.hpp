#pragma once

#include <CppUtils/Language/Lexer/GrammarLexer.hpp>

namespace CppUtils::Language::Lexer
{
	namespace Parameters
	{
		struct Command final
		{
			std::string_view name;
			std::function<bool(std::string_view)> function;
		};
	}

	class ParametersLexer final
	{
	public:
		ParametersLexer()
		{
			using namespace std::literals;
			using namespace Type::Literals;

			m_grammarLexer.addParserFunction("spaceParser"_token, Parser::spaceParser);
			m_grammarLexer.addParserFunction("keywordParser"_token, Parser::keywordParser);
			m_grammarLexer.addParserFunction("valueParser"_token, [](auto& cursor, auto& parentNode) {
				cursor.skipSpaces();
				if (!cursor.isEndOfString() && cursor.getChar() != '[')
					return false;
				const auto startPos = ++cursor.pos;
				while (!cursor.isEndOfString() && cursor.getChar() != ']')
					++cursor.pos;
				if (cursor.isEndOfString())
					return false;
				auto stringToken = Type::Token{String::trimString(cursor.src.substr(startPos, cursor.pos - startPos))};
				++cursor.pos;
				stringToken.saveTypename();
				parentNode.childs.emplace_back(Graph::TokenNode{std::move(stringToken)});
				return true;
			});

			static constexpr auto grammarSrc = "\
			main: (command >= 0) spaceParser;\
			command: spaceParser keywordParser !_value;\
			_value: spaceParser valueParser;\
			"sv;
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] inline std::unordered_map<std::string, std::string> parseParameters(const std::size_t argc, const char *argv[]) const
		{
			
			const auto parameters = String::cstringArrayToVectorOfStrings<std::string_view>(argv + 1, argc - 1);
			const auto src = String::concatenateStringsWithDelimiter(parameters, " ");
			const auto commandTree = m_grammarLexer.parseLanguage(src);

			auto map = std::unordered_map<std::string, std::string>{};
			for (const auto& command : commandTree.childs)
				map[std::string{command.childs.at(0).self.name}] = (command.childs.size() == 2 ? command.childs.at(1).self.name : "");
			return map;
		}

		[[nodiscard]] inline bool executeCommands(const std::size_t argc, const char *argv[], const std::vector<Parameters::Command>& commands) const
		{
			const auto parameters = parseParameters(argc, argv);
			for (auto const& command : commands)
				if (parameters.find(command.name.data()) != parameters.end() && command.function(parameters.at(command.name.data())))
					return true;
			return false;
		}

	private:
		GrammarLexer m_grammarLexer;
	};
}
