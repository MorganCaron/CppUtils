#pragma once

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

			m_grammarLexer.addParsingFunction("spaceParser"_token, Language::Parser::spaceParser<Type::Token>);
			m_grammarLexer.addParsingFunction("keywordParser"_token, Language::Parser::keywordParser<Type::Token>);
			m_grammarLexer.addParsingFunction("valueParser"_token, [](auto& context) {
				auto& [cursor, parentNode, firstChildPosition] = context;
				if (!cursor.isEndOfString() && cursor.getChar() != '[')
					return false;
				const auto startPosition = ++cursor.position;
				while (!cursor.isEndOfString() && cursor.getChar() != ']')
					++cursor.position;
				if (cursor.isEndOfString())
					return false;
				auto stringToken = Type::Token{String::trimString(cursor.src.substr(startPosition, cursor.position - startPosition))};
				++cursor.position;
				stringToken.saveTypename();
				parentNode.get().childs.emplace_back(Language::Parser::ASTNode<Type::Token>{std::move(stringToken)});
				return true;
			});

			static constexpr auto grammarSrc = R"(
			main: (command >= 0) spaceParser;
			command: spaceParser keywordParser ~value;
			!value: spaceParser valueParser;
			)"sv;
			m_grammarLexer.parseGrammar(grammarSrc);
		}

		[[nodiscard]] inline std::unordered_map<std::string, std::string> parseParameters(const std::size_t argc, const char *argv[]) const
		{
			using namespace Type::Literals;
			const auto parameters = String::cstringArrayToVectorOfStrings<std::string_view>(argv + 1, argc - 1);
			const auto src = String::concatenateStringsWithDelimiter(parameters, " ");
			const auto commandTree = m_grammarLexer.parseLanguage("main"_token, src);

			auto map = std::unordered_map<std::string, std::string>{};
			for (const auto& command : commandTree.childs)
				map[std::string{std::get<Type::Token>(command.childs.at(0).value).name}] = (command.childs.size() == 2 ? std::get<Type::Token>(command.childs.at(1).value).name : "");
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
		Language::Lexer::GrammarLexer<Type::Token> m_grammarLexer;
	};
}
