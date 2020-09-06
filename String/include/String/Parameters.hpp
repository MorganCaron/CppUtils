#pragma once

#include <cctype>

#include <String/String.hpp>

namespace CppUtils::String::Parameters
{
	[[nodiscard]] inline std::unordered_map<std::string, std::string> parseParameters(const std::size_t argc, const char *argv[])
	{
		const auto parameters = cstringArrayToVectorOfStrings(argv + 1, argc - 1);
		const auto command = concatenateStringsWithDelimiter(parameters, " ");

		const auto skipSpaces = [](std::string_view src, std::size_t& pos) -> void {
			while (pos < src.size() && std::isspace(src.at(pos)))
				++pos;
		};

		const auto getWord = [](std::string_view src, std::size_t& pos) -> std::string {
			auto wordLength = 0u;
			while (pos + wordLength < src.size() && std::isgraph(src.at(pos + wordLength)) && src.at(pos + wordLength) != '[' && src.at(pos + wordLength) != ']')
				++wordLength;
			auto word = src.substr(pos, wordLength);
			pos += wordLength;
			if (wordLength == 0)
				throw std::runtime_error{"Incorrectly formatted command"};
			return std::string{word};
		};

		auto map = std::unordered_map<std::string, std::string>{};
		auto pos = std::size_t{0};

		skipSpaces(command, pos);
		while (pos < command.size())
		{
			const auto parameter = getWord(command, pos);
			skipSpaces(command, pos);
			if (pos < command.size() && command.at(pos) == '[')
			{
				++pos;
				skipSpaces(command, pos);
				map[parameter] = getWord(command, pos);
				skipSpaces(command, pos);
				if (pos >= command.size() || command.at(pos) != ']')
					throw std::runtime_error{"Missing parenthesis closure in the parameters passed to the executable"};
				++pos;
			}
			else
				map[parameter] = "";
			skipSpaces(command, pos);
		}

		return map;
	}
	
}
