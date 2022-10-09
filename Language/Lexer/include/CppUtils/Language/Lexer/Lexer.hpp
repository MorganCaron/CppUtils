#pragma once

#include <CppUtils/Hash/Token.hpp>
#include <CppUtils/String/String.hpp>
#include <CppUtils/Language/Lexer/Context.hpp>

namespace CppUtils::Language::Lexer
{
	template<class Element>
	bool parseLexeme(Context<Element>& context);

	template<class Element>
	bool parseLexemes(Context<Element>& context)
	{
		for (const auto& node : context.lexeme.get().nodes)
		{
			context.lexeme = node;
			if (!parseLexeme(context))
				return false;
		}
		return true;
	}

	template<class Element>
	[[nodiscard]] constexpr bool parseDeclaration(Context<Element>& context)
	{
		auto& [parserContext, declaration, grammar] = context;
		auto& [cursor, parentNode, ast] = parserContext;
		auto newParentNode = parentNode.get();
		auto commaPosition = std::size_t{0};
		const auto& lexemes = declaration.get().nodes;
		auto nbLexemes = std::size(lexemes);
		for (auto i = 0u; i < nbLexemes; ++i)
		{
			const auto& lexeme = lexemes[i];
			if (lexeme.value == ',')
				commaPosition = i;
			else if (context.lexeme = lexeme; !parseLexeme(context))
			{
				if (commaPosition != 0)
					throw std::logic_error{"Unknown element"};
				parentNode.get() = newParentNode;
				return false;
			}
		}
		return true;
	}
	
	template<class Element>
	[[nodiscard]] constexpr bool parseMultipleDeclaration(Context<Element>& context)
	{
		auto& [parserContext, declarationLexeme, grammar] = context;
		auto& [cursor, parentNode, ast] = parserContext;
		auto declarationToken = declarationLexeme.get().value;
		for (const auto& declaration : grammar.get().root.getNodesWithValue(declarationToken))
		{
			auto newContext = context;
			newContext.lexeme = declaration;
			if (parseDeclaration(newContext))
			{
				cursor.pos = newContext.parserContext.cursor.pos;
				return true;
			}
		}
		return false;
	}

	template<class Element>
	bool parseLexeme(Context<Element>& context)
	{
		using namespace std::literals;
		using namespace Hash::Literals;
		auto& [parserContext, lexeme, grammar] = context;
		auto& [cursor, parentNode, ast] = parserContext;
		auto startPos = cursor.pos;
		switch (lexeme.get().value)
		{
			case "optional"_token:
			{
				if (lexeme.get().nodes.empty())
					throw std::logic_error{"Missing element in 'optional' token"};
				context.lexeme = lexeme.get().nodes[0];
				parseLexeme(context);
				return true;
			}
			case "token"_token:
			{
				if (lexeme.get().nodes.empty())
					throw std::logic_error{"Missing element in 'token' token"};
				context.lexeme = lexeme.get().nodes[0];
				return parseMultipleDeclaration(context);
			}
			case "string"_token:
			{
				auto size = std::size(lexeme.get().nodes);
				if (size == 0)
					throw std::logic_error{"Missing element in 'string' token"};
				if (cursor.pos + size > std::size(cursor.data))
					return false;
				for (auto i = 0u; i < size; ++i)
				{
					if (auto c = lexeme.get().nodes[i].value; cursor.is(static_cast<Element>(c)))
						++cursor.pos;
					else
						return false;
				}
				return true;
			}
			case "read"_token:
			{
				if (cursor.pos == std::size(cursor.data))
					return false;
				parentNode.get().nodes.push_back(Parser::AstNode{static_cast<std::uintptr_t>(cursor.current())});
				return true;
			}
			case "add"_token:
			{
				if (lexeme.get().nodes.empty())
					throw std::logic_error{"Missing element in 'add' token"};
				const auto& nodeToAdd = lexeme.get().nodes[0];
				if (nodeToAdd.value == "token"_token && !nodeToAdd.nodes.empty())
				{
					auto tokenName = ""s;
					for (const auto& charNode : nodeToAdd.nodes)
						tokenName += charNode.value;
					parentNode.get().nodes.push_back(Parser::AstNode{Hash::hash(tokenName)});
				}
				else
					parentNode.get().nodes.push_back(nodeToAdd);
				return true;
			}
			case "or"_token:
			{
				for (const auto& node : lexeme.get().nodes)
				{
					context.lexeme = node;
					if (parseLexeme(context))
						return true;
				}
				cursor.pos = startPos;
				return false;
			}
			case "hash"_token:
			{
				auto newContext = context;
				newContext.lexeme = lexeme.get().nodes[0];
				auto tempParentNode = Parser::AstNode{parentNode.get().value};
				newContext.parserContext.parentNode = tempParentNode;
				if (!parseLexeme(newContext))
					return false;
				auto string = ""s;
				for (const auto& node: tempParentNode.nodes)
					string += static_cast<char>(node.value);
				auto token = Hash::hash(string);
				ast.tokenNames.get()[token] = std::move(string);
				parentNode.get().nodes.push_back(Parser::AstNode{token});
				cursor.pos = newContext.parserContext.cursor.pos;
				return true;
			}
			case "sub"_token:
			{
				if (lexeme.get().nodes.empty())
					throw std::logic_error{"Missing element in 'sub' token"};
				auto oldParentNode = parentNode;
				parentNode = parentNode.get().nodes[std::size(parentNode.get().nodes) - 1];
				auto result = parseLexemes(context);
				parentNode = oldParentNode;
				return result;
			}
			case ">="_token: return cursor.pos < std::size(cursor.data) && cursor[cursor.pos] >= static_cast<char>(lexeme.get().nodes[0].value);
			case "<="_token: return cursor.pos < std::size(cursor.data) && cursor[cursor.pos] <= static_cast<char>(lexeme.get().nodes[0].value);
			case '+':
			{
				if (cursor.pos == std::size(cursor.data))
					throw std::logic_error{"Can't increment the cursor, it has reached the end of the string"};
				++cursor.pos;
				return true;
			}
			case "not"_token:
			{
				if (lexeme.get().nodes.empty())
					throw std::logic_error{"Missing element in 'not' token"};
				auto newContext = context;
				newContext.lexeme = lexeme.get().nodes[0];
				auto result = parseLexeme(newContext);
				cursor.pos = startPos;
				return !result;
			}
			case "parenthesis"_token:
				return parseLexemes(context);
			case "end"_token:
				return cursor.pos == std::size(cursor.data);
		}
		throw std::logic_error{"Unknown lexeme " + Hash::getTokenNameOrValue(lexeme.get().value, grammar.get().tokenNames)};
		return false;
	}

	template<class CharT>
	[[nodiscard]] Parser::Ast parse(std::basic_string_view<CharT> src, const Parser::Ast& grammar)
	{
		if (grammar.root.nodes.empty())
			throw std::logic_error{"Empty grammar"};
		auto ast = Parser::Ast{grammar.tokenNames};
		auto declarationIt = std::find_if(grammar.root.nodes.cbegin(), grammar.root.nodes.cend(), [](const auto& declaration) -> bool {
			using namespace Hash::Literals;
			return declaration.value == "main"_token;
		});
		if (declarationIt == grammar.root.nodes.cend())
			throw std::logic_error{"Entry point \"main\" not found"};
		auto mainLexeme = *declarationIt;
		auto context = Context<CharT>{
			.parserContext = Parser::Context<CharT>{
				.cursor = Parser::Cursor{src},
				.parentNode = ast.root,
				.ast = Parser::ReadableAstNode{ast}
			},
			.lexeme = mainLexeme,
			.grammar = grammar
		};
		try
		{
			if (!parseDeclaration(context) || !context.parserContext.cursor.isEnd())
				throw std::logic_error{"Unknown element"};
		}
		catch (const std::exception& exception)
		{
			const auto& cursor = context.parserContext.cursor;
			std::throw_with_nested(std::runtime_error{getPositionInformation(cursor) + ": '" + String::reverseEscapedChar(cursor.current()) + '\''});
		}
		return ast;
	}
}
