#pragma once

#include <cctype>
#include <functional>

#include <String/String.hpp>
#include <Parser/Cursor.hpp>

namespace CppUtils::Terminal::Parameters
{
	static constexpr const auto Delimiters = std::pair<char, char>{'[', ']'};

	[[nodiscard]] inline std::unordered_map<std::string, std::string> parseParameters(const std::size_t argc, const char *argv[])
	{
		static const auto parseCommand = [](Parser::Cursor& cursor) -> std::string {
			auto wordLength = 0u;
			while (cursor.pos + wordLength < cursor.src.size() &&
				std::isgraph(cursor.src.at(cursor.pos + wordLength)) &&
				cursor.src.at(cursor.pos + wordLength) != Delimiters.first &&
				cursor.src.at(cursor.pos + wordLength) != Delimiters.second)
				++wordLength;
			auto word = cursor.src.substr(cursor.pos, wordLength);
			cursor.pos += wordLength;
			if (wordLength == 0)
				throw std::runtime_error{"A command name is expected"};
			return std::string{word};
		};

		static const auto parseValue = [](Parser::Cursor& cursor) -> std::string {
			auto wordLength = 0u;
			while (cursor.pos + wordLength < cursor.src.size() &&
				cursor.src.at(cursor.pos + wordLength) != Delimiters.first &&
				cursor.src.at(cursor.pos + wordLength) != Delimiters.second)
				++wordLength;
			while (wordLength > 0 &&
				cursor.pos + wordLength < cursor.src.size() &&
				std::isspace(cursor.src.at(cursor.pos + wordLength - 1)))
				--wordLength;
			auto word = cursor.src.substr(cursor.pos, wordLength);
			cursor.pos += wordLength;
			return std::string{word};
		};

		const auto parameters = String::cstringArrayToVectorOfStrings(argv + 1, argc - 1);
		const auto src = String::concatenateStringsWithDelimiter(parameters, " ");
		auto pos = std::size_t{0};
		auto cursor = Parser::Cursor{src, pos};
		auto map = std::unordered_map<std::string, std::string>{};

		cursor.skipSpaces();
		while (!cursor.isEndOfString())
		{
			const auto parameter = parseCommand(cursor);
			cursor.skipSpaces();
			if (!cursor.isEndOfString() && cursor.getChar() == Delimiters.first)
			{
				++cursor.pos;
				cursor.skipSpaces();
				map[parameter] = parseValue(cursor);
				cursor.skipSpaces();
				if (cursor.isEndOfString() || cursor.getChar() != Delimiters.second)
					throw std::runtime_error{"Missing parenthesis closure in the parameters passed to the executable"};
				++cursor.pos;
			}
			else
				map[parameter] = "";
			cursor.skipSpaces();
		}

		return map;
	}

	struct Command
	{
		std::string_view name;
		std::function<bool(std::string_view)> function;
	};
	
	[[nodiscard]] inline bool executeCommands(const std::size_t argc, const char *argv[], const std::vector<Command>& commands)
	{
		const auto parameters = parseParameters(argc, argv);
		for (auto const& command : commands)
			if (parameters.find(command.name.data()) != parameters.end() && command.function(parameters.at(command.name.data())))
				return true;
		return false;
	}
}
