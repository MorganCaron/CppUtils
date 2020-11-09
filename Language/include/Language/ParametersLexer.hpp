#pragma once

#include <Language/GrammarLexer.hpp>

namespace CppUtils::Language::Parameters
{
	struct Command
	{
		std::string_view name;
		std::function<bool(std::string_view)> function;
	};

	class ParametersLexer final
	{
	public:
		ParametersLexer()
		{
			using namespace Type::Literals;

			m_grammarLexer.addParserFunction("spaceParser"_typeId, Language::Parser::spaceParser);
			m_grammarLexer.addParserFunction("keywordParser"_typeId, Language::Parser::keywordParser);
			m_grammarLexer.addParserFunction("valueParser"_typeId, [](auto& cursor, auto& parentNode) {
				cursor.skipSpaces();
				if (!cursor.isEndOfString() && cursor.getChar() != '[')
					return false;
				const auto startPos = ++cursor.pos;
				while (!cursor.isEndOfString() && cursor.getChar() != ']')
					++cursor.pos;
				if (cursor.isEndOfString())
					return false;
				auto stringToken = Language::Lexeme::Token{String::trimString(cursor.src.substr(startPos, cursor.pos - startPos))};
				++cursor.pos;
				stringToken.saveTypename();
				parentNode.childs.emplace_back(Language::Lexeme::TokenNode{std::move(stringToken)});
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
			
			const auto parameters = String::cstringArrayToVectorOfStrings(argv + 1, argc - 1);
			const auto src = String::concatenateStringsWithDelimiter(parameters, " ");
			const auto commandTree = m_grammarLexer.parseLanguage(src);

			auto map = std::unordered_map<std::string, std::string>{};
			for (const auto& command : commandTree.childs)
				map[std::string{command.childs.at(0).self.name}] = (command.childs.size() == 2 ? command.childs.at(1).self.name : "");
			return map;
		}

		[[nodiscard]] inline bool executeCommands(const std::size_t argc, const char *argv[], const std::vector<Command>& commands) const
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
