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
			++cursor.position;
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
					cursor.position = newContext.cursor.position;
					if (!cursor.is('}'))
						throw std::logic_error{"Missing '}'"};
					++cursor.position;
					break;
				}
				case '_':
				{
					auto nbLetters = std::size(tokenNameBuffer);
					if (nbLetters == 0)
						throw std::logic_error{"Token name missing before '_' char"};
					auto token = Hash::hash(tokenNameBuffer);
					ast.tokenNames.get()[token] = std::move(tokenNameBuffer);
					tokenNameBuffer.clear();
					parentNode.get().nodes.erase(std::cend(parentNode.get().nodes) - nbLetters, std::cend(parentNode.get().nodes));
					parentNode.get().nodes.push_back(Graph::Tree::Node{token});
					break;
				}
				case '\\':
				{
					if (cursor.isEnd())
						throw std::logic_error{"Missing character after '\\'"};
					value = cursor.read();
					if (auto it = String::escapedChars.find(value); it != std::cend(String::escapedChars))
						value = it->second;
					[[fallthrough]];
				}
				default:
					parentNode.get().nodes.emplace_back(Graph::Tree::Node{static_cast<std::uintptr_t>(value)});
					tokenNameBuffer += static_cast<char>(value);
			}
		}
	}

	template<class CharT>
	[[nodiscard]] constexpr Ast parseAst(std::basic_string_view<CharT> src)
	{
		auto ast = Ast{"root"};
		auto context = Context<CharT>{
			.cursor = Cursor{src},
			.parentNode = ast.root,
			.ast = Parser::AstRef{ast}
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
	
	namespace Literals
	{
		[[nodiscard]] auto operator"" _ast(const char* cString) -> Ast
		{
			return parseAst<char>(cString);
		}
	}
}
