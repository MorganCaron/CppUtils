#pragma once

#include <CppUtils/Hash/Token.hpp>
#include <CppUtils/String/String.hpp>
#include <CppUtils/Language/Parser/Ast.hpp>
#include <CppUtils/Language/Parser/Context.hpp>

namespace CppUtils::Language::Parser
{
	template<typename Element = char>
	constexpr void parseAst(Context<Element>& context)
	{
		auto& [cursor, parentNode, ast] = context;
		auto tokenNameBuffer = std::string{};
		for (char value; !cursor.isEnd() && (value = cursor.current()) != '}'; )
		{
			++cursor.pos;
			switch (value)
			{
				case ' ': case '\n': case '\t': case '\r':
					tokenNameBuffer.clear();
					break;
				case '{':
				{
					tokenNameBuffer.clear();
					auto newContext = Context<Element>{cursor, parentNode.get().nodes.back(), ast};
					parseAst(newContext);
					cursor.pos = newContext.cursor.pos;
					if (!cursor.is('}'))
						throw std::logic_error{"Missing '}'"};
					++cursor.pos;
					break;
				}
				case '_':
				{
					auto nbLetters = tokenNameBuffer.size();
					if (nbLetters == 0)
						throw std::logic_error{"Token name missing before '_' char"};
					auto token = Hash::hash(tokenNameBuffer);
					ast.tokenNames.get()[token] = std::move(tokenNameBuffer);
					tokenNameBuffer.clear();
					parentNode.get().nodes.erase(parentNode.get().nodes.cend() - nbLetters, parentNode.get().nodes.cend());
					parentNode.get().nodes.push_back(Graph::Tree::Node{token});
					break;
				}
				case '\\':
				{
					if (cursor.isEnd())
						throw std::logic_error{"Missing character after '\\'"};
					value = cursor.read();
					if (String::escapedChars.contains(value))
						value = String::escapedChars.at(value);
					[[fallthrough]];
				}
				default:
					parentNode.get().nodes.emplace_back(Graph::Tree::Node{static_cast<std::uintptr_t>(value)});
					tokenNameBuffer += static_cast<char>(value);
			}
		}
	}

	[[nodiscard]] Ast parseAst(std::string_view src)
	{
		auto ast = Ast{};
		auto context = Context<char>{
			.cursor = Cursor{src},
			.parentNode = ast.root,
			.ast = Parser::ReadableAstNode{ast}
		};
		try
		{
			parseAst(context);
			if (!context.cursor.isEnd())
				throw std::logic_error{"Unknown element"};
		}
		catch (const std::exception& exception)
		{
			std::throw_with_nested(std::runtime_error{getPositionInformation(context.cursor) + ": '" + context.cursor.current() + '\''});
		}
		return ast;
	}
}
