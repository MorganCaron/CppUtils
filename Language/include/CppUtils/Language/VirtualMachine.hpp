#pragma once

#include <CppUtils/Language/Ast.hpp>

namespace CppUtils::Language::VirtualMachine
{
	[[nodiscard]] inline auto getTokenNames() -> const Hash::TokenNames&
	{
		using namespace Hash::Literals;
		static const auto tokenNames = Hash::TokenNames{
			{ "token"_token, "token" },
			{ "source"_token, "source" },
			{ "stack"_token, "stack" },
			{ "register"_token, "register" },
			{ "rodata"_token, "rodata" },
			{ "index"_token, "index" },
			{ "reverseIndex"_token, "reverseIndex" },
			{ "value"_token, "value" },
			{ "deref"_token, "deref" },
			{ "main"_token, "main" },
			{ "call"_token, "call" },
			{ "move"_token, "move" },
			{ "xor"_token, "xor" },
			{ "add"_token, "add" },
			{ "push"_token, "push" },
			{ "pop"_token, "pop" },
			{ "cmp"_token, "cmp" },
			{ "sete"_token, "sete" },
			{ "return"_token, "return" },
			{ "FLAGS"_token, "FLAGS" },
			{ "R0"_token, "R0" },
			{ "R1"_token, "R1" }
		};
		return tokenNames;
	}

	[[nodiscard]] inline auto getFirstNode(AstNode& node) -> AstNode&
	{
		return std::empty(node.nodes) ? node.nodes.emplace_back() : node.nodes.front();
	}

	[[nodiscard]] inline auto getTargetNode(AstNode& contextNode, const AstNode& addressNode) -> AstNode&
	{
		using namespace Hash::Literals;
		auto subNodeValue = addressNode.nodes.front().value;
		switch (addressNode.value)
		{
			case "token"_token: return contextNode[subNodeValue];
			case "index"_token: return contextNode.nodes[subNodeValue];
			case "reverseIndex"_token: return contextNode.nodes[std::size(contextNode.nodes) - 1 - subNodeValue];
			default: throw std::runtime_error{"Incorrect address format: Unknown node type"};
		}
	}

	[[nodiscard]] inline auto resolveAddress(AstNode& contextNode, const AstNode& addressNode) -> AstNode&
	{
		auto& target = getTargetNode(contextNode, addressNode);
		if (const auto& subNodes = addressNode.nodes.front().nodes; !std::empty(subNodes))
			return resolveAddress(target, subNodes.front());
		return target;
	}

	[[nodiscard]] inline auto getValueNode(AstNode& contextNode, const AstNode& addressNode) -> const AstNode&
	{
		using namespace Hash::Literals;
		if (addressNode.value == "value"_token)
			return addressNode.nodes.front();
		return resolveAddress(contextNode, addressNode);
	}

	[[nodiscard]] inline auto getValue(AstNode& contextNode, const AstNode& addressNode) -> Hash::Token
	{
		using namespace Hash::Literals;
		if (addressNode.value == "deref"_token)
			return *reinterpret_cast<const char*>(resolveAddress(contextNode, addressNode.nodes.front()).value);
		return getValueNode(contextNode, addressNode).value;
	}

	[[nodiscard]] inline auto run(Ast& context, Hash::Token entryPoint = Hash::hash(std::string_view{"main"})) -> const AstNode&
	{
		using namespace Hash::Literals;
		const auto& declarations = context.root["source"_token];
		auto declarationIt = declarations.find(entryPoint);
		if (declarationIt == std::cend(declarations.nodes))
			return getFirstNode(context.root["register"_token]["R0"_token]);
		const auto& lexemes = declarationIt->nodes;
		for (const auto& lexeme : lexemes)
		{
			switch (lexeme.value)
			{
				case "call"_token: return run(context, context.root["stack"_token].nodes.back().value); // return ? ça quitte le for
				case "move"_token: getFirstNode(resolveAddress(context.root, lexeme.nodes[0])).value = getValue(context.root, lexeme.nodes[1]); break;
				case "xor"_token: getFirstNode(resolveAddress(context.root, lexeme.nodes[0])).value ^= getValue(context.root, lexeme.nodes[1]); break;
				case "add"_token: getFirstNode(resolveAddress(context.root, lexeme.nodes[0])).value += getValue(context.root, lexeme.nodes[1]); break;
				case "push"_token: resolveAddress(context.root, lexeme.nodes[0]).nodes.push_back(getValueNode(context.root, lexeme.nodes[1])); break;
				case "pop"_token: resolveAddress(context.root, lexeme.nodes[0]).nodes.pop_back(); break;
				case "cmp"_token: getFirstNode(context.root["register"_token]["FLAGS"_token]).value = getValue(context.root, lexeme.nodes[0]) == getValue(context.root, lexeme.nodes[1]); break;
				case "sete"_token: getFirstNode(resolveAddress(context.root, lexeme.nodes[0])).value = context.root["register"_token]["FLAGS"_token].nodes.front().value; break;
				case "return"_token: return getFirstNode(context.root["register"_token]["R0"_token]);
				default: throw std::runtime_error{"Unknown bytecode instruction"};
			}
		}
		throw std::runtime_error{"Missing return instruction"};
	}
}
