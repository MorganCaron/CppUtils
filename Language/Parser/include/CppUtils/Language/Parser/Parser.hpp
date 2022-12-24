#pragma once

#include <CppUtils/Language/Parser/Ast.hpp>
#include <CppUtils/Language/Parser/Cursor.hpp>

namespace CppUtils::Language::Parser
{
	[[nodiscard]] inline auto getGrammarNames() -> const Hash::TokenNames&
	{
		using namespace Hash::Literals;
		static const auto grammarNames = Hash::TokenNames{
			{ "main"_token, "main" },
			{ "read"_token, "read" },
			{ "compare"_token, "compare" },
			{ "hash"_token, "hash" },
			{ "call"_token, "call" },
			{ "increment"_token, "increment" },
			{ "add"_token, "add" },
			{ "push"_token, "push" },
			{ "pop"_token, "pop" },
			{ "drop"_token, "drop" },
			{ "size"_token, "size" },
			{ "set"_token, "set" },
			{ "eof"_token, "eof" },
			{ "not"_token, "not" },
			{ "return"_token, "return" },
			{ "returnIf"_token, "returnIf" },
			{ "returnIfNot"_token, "returnIfNot" }
		};
		return grammarNames;
	}

	template<class CharT>
	[[nodiscard]] constexpr auto parse(const Parser::Ast& grammar, Parser::Cursor<CharT>& cursor, Parser::AstRef& context, Hash::Token lexemeId = Hash::hash(std::string_view{"main"})) -> Hash::Token
	{
		using namespace Hash::Literals;
		const auto& declarations = grammar.root.nodes;
		auto declarationIt = std::find_if(std::cbegin(declarations), std::cend(declarations), [lexemeId](const auto& declaration) -> bool {
			return declaration.value == lexemeId;
		});
		if (declarationIt == std::cend(declarations))
			return 0;
		auto lexemes = declarationIt->nodes;
		auto hold = Hash::Token{};
		auto& contextNodes = context.node.get().nodes;
		auto& tokenNames = context.tokenNames.get();
		auto backStack = [&contextNodes]() -> Parser::AstNode& { if (std::empty(contextNodes)) std::runtime_error{"Cannot access empty stack"}; return contextNodes[std::size(contextNodes) - 1]; };
		auto popStack = [&contextNodes, &backStack]() -> Hash::Token { auto back = backStack().value; contextNodes.pop_back(); return back; };
		auto getStackSize = [&contextNodes]() -> Hash::Token { return std::size(contextNodes); };
		for (const auto& lexeme : lexemes)
		{
			switch (lexeme.value)
			{
			case "read"_token: hold = static_cast<Hash::Token>(cursor.current()); break;
			case "compare"_token: hold = hold == backStack().value; break;
			case "hash"_token: hold = ((hold ? hold : 0xcbf29ce484222325u) ^ popStack()) * 0x100000001b3u; break;
			case "call"_token: { auto astRef = Parser::AstRef{backStack(), tokenNames}; hold = parse(grammar, cursor, astRef, hold); break; }
			case "increment"_token: ++cursor.position; break;
			case "add"_token: hold += backStack().value; break;
			case "push"_token: contextNodes.emplace_back(hold); break;
			case "pop"_token: hold = popStack(); break;
			case "drop"_token: popStack(); break;
			case "size"_token: hold = getStackSize(); break;
			case "set"_token: hold = lexeme.nodes[0].value; break;
			case "eof"_token: hold = cursor.position >= std::size(cursor.data); break;
			case "not"_token: hold = !hold; break;
			case "return"_token: return hold;
			case "returnIf"_token: if (hold) return hold; break;
			case "returnIfNot"_token: if (!hold) return false; break;
			default: throw std::runtime_error{"Unknown bytecode instruction"};
			}
		}
		throw std::runtime_error{"Syntax error"};
	}

	template<class CharT>
	[[nodiscard]] constexpr auto parse(const Parser::Ast& grammar, Parser::Cursor<CharT>& cursor, std::string outputName = "Output", Hash::Token lexemeId = Hash::hash(std::string_view{"main"})) -> Parser::Ast
	{
		auto context = Parser::Ast{std::move(outputName)};
		auto contextRef = Parser::AstRef{context};
		if (!parse(grammar, cursor, contextRef, lexemeId))
			throw std::runtime_error{"Parsing error"};
		return context;
	}
}
