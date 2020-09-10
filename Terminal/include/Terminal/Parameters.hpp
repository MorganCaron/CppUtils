#pragma once

#include <cctype>
#include <functional>

#include <String/String.hpp>

namespace CppUtils::Terminal::Parameters
{
	static constexpr const auto Delimiters = std::pair<char, char>{'[', ']'};

	[[nodiscard]] inline std::unordered_map<std::string, std::string> parseParameters(const std::size_t argc, const char *argv[])
	{
		const auto parameters = String::cstringArrayToVectorOfStrings(argv + 1, argc - 1);
		const auto command = String::concatenateStringsWithDelimiter(parameters, " ");

		const auto skipSpaces = [](std::string_view src, std::size_t& pos) -> void {
			while (pos < src.size() && std::isspace(src.at(pos)))
				++pos;
		};

		const auto parseCommand = [](std::string_view src, std::size_t& pos) -> std::string {
			auto wordLength = 0u;
			while (pos + wordLength < src.size() && std::isgraph(src.at(pos + wordLength)) && src.at(pos + wordLength) != Delimiters.first && src.at(pos + wordLength) != Delimiters.second)
				++wordLength;
			auto word = src.substr(pos, wordLength);
			pos += wordLength;
			if (wordLength == 0)
				throw std::runtime_error{"A command name is expected"};
			return std::string{word};
		};

		const auto parseValue = [](std::string_view src, std::size_t& pos) -> std::string {
			auto wordLength = 0u;
			while (pos + wordLength < src.size() && src.at(pos + wordLength) != Delimiters.first && src.at(pos + wordLength) != Delimiters.second)
				++wordLength;
			while (wordLength > 0 && pos + wordLength < src.size() && std::isspace(src.at(pos + wordLength - 1)))
				--wordLength;
			auto word = src.substr(pos, wordLength);
			pos += wordLength;
			return std::string{word};
		};

		auto map = std::unordered_map<std::string, std::string>{};
		auto pos = std::size_t{0};

		skipSpaces(command, pos);
		while (pos < command.size())
		{
			const auto parameter = parseCommand(command, pos);
			skipSpaces(command, pos);
			if (pos < command.size() && command.at(pos) == Delimiters.first)
			{
				++pos;
				skipSpaces(command, pos);
				map[parameter] = parseValue(command, pos);
				skipSpaces(command, pos);
				if (pos >= command.size() || command.at(pos) != Delimiters.second)
					throw std::runtime_error{"Missing parenthesis closure in the parameters passed to the executable"};
				++pos;
			}
			else
				map[parameter] = "";
			skipSpaces(command, pos);
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
