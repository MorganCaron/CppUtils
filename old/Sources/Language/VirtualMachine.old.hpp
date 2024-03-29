#pragma once

#include <CppUtils/Language/Ast.hpp>

namespace CppUtils::Language::VirtualMachine
{
	[[nodiscard]] inline auto getTokenNames() -> const String::HashTable&
	{
		using namespace String::Literals;
		static const auto tokenNames = String::HashTable{
			{"token"_token, "token"},
			{"source"_token, "source"},
			{"stack"_token, "stack"},
			{"register"_token, "register"},
			{"rodata"_token, "rodata"},
			{"index"_token, "index"},
			{"reverseIndex"_token, "reverseIndex"},
			{"value"_token, "value"},
			{"deref"_token, "deref"},
			{"call"_token, "call"},
			{"externalCall"_token, "externalCall"},
			{"move"_token, "move"},
			{"xor"_token, "xor"},
			{"add"_token, "add"},
			{"push"_token, "push"},
			{"pop"_token, "pop"},
			{"cmp"_token, "cmp"},
			{"sete"_token, "sete"},
			{"return"_token, "return"},
			{"FLAGS"_token, "FLAGS"},
			{"LibSystem"_token, "LibSystem"},
			{"allocateMemory"_token, "allocateMemory"},
			{"freeMemory"_token, "freeMemory"},
			{"main"_token, "main"},
			{"parseToken"_token, "parseToken"},
			{"parseNode"_token, "parseNode"}};
		return tokenNames;
	}

	[[nodiscard]] inline auto getTargetNode(AstNode& contextNode, const AstNode& addressNode) -> AstNode&
	{
		using namespace String::Literals;
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
		using namespace String::Literals;
		if (addressNode.value == "value"_token)
			return addressNode.nodes.front();
		return resolveAddress(contextNode, addressNode);
	}

	[[nodiscard]] inline auto getValue(AstNode& contextNode, const AstNode& addressNode) -> Token
	{
		using namespace String::Literals;
		if (addressNode.value == "deref"_token)
		{
			const auto address = resolveAddress(contextNode, addressNode.nodes.front()).value;
			return *reinterpret_cast<const char*>(address);
		}
		return getValueNode(contextNode, addressNode).value;
	}

	inline auto callExternalFunction(Ast& context, const AstNode& addressNode) -> void
	{
		using namespace String::Literals;
		auto functionAddress = Ast::getFirstNode(resolveAddress(context.root["source"_token], addressNode)).value;
		auto* functionPointer = reinterpret_cast<void (*)(Ast&)>(functionAddress);
		functionPointer(context);
	}

	inline auto run(Ast& context, String::Token entryPoint = String::Hash::hash(std::string_view{"main"})) -> void
	{
		using namespace String::Literals;
		const auto& declarations = context.root["source"_token];
		auto declarationIt = declarations.find(entryPoint);
		if (declarationIt == std::cend(declarations.nodes))
			throw std::runtime_error{"Unknown label"};
		const auto& lexemes = declarationIt->nodes;
		for (const auto& lexeme : lexemes)
		{
			switch (lexeme.value)
			{
			case "call"_token: run(context, resolveAddress(context.root["source"_token], lexeme.nodes[0]).value); break;
			case "externalCall"_token: callExternalFunction(context, lexeme.nodes[0]); break;
			case "move"_token: Ast::getFirstNode(resolveAddress(context.root, lexeme.nodes[0])).value = getValue(context.root, lexeme.nodes[1]); break;
			case "xor"_token: Ast::getFirstNode(resolveAddress(context.root, lexeme.nodes[0])).value ^= getValue(context.root, lexeme.nodes[1]); break;
			case "add"_token: Ast::getFirstNode(resolveAddress(context.root, lexeme.nodes[0])).value += getValue(context.root, lexeme.nodes[1]); break;
			case "push"_token: resolveAddress(context.root, lexeme.nodes[0]).nodes.push_back(getValueNode(context.root, lexeme.nodes[1])); break;
			case "pop"_token: resolveAddress(context.root, lexeme.nodes[0]).nodes.pop_back(); break;
			case "cmp"_token: Ast::getFirstNode(context.root["register"_token]["FLAGS"_token]).value = getValue(context.root, lexeme.nodes[0]) == getValue(context.root, lexeme.nodes[1]); break;
			case "sete"_token: Ast::getFirstNode(resolveAddress(context.root, lexeme.nodes[0])).value = context.root["register"_token]["FLAGS"_token].nodes.front().value; break;
			case "return"_token: return;
			default: throw std::runtime_error{"Unknown bytecode instruction"};
			}
		}
	}

	[[nodiscard]] inline auto getReturnNode(Ast& context) -> AstNode&
	{
		using namespace String::Literals;
		return Ast::getFirstNode(context.root["register"_token][0]);
	}
}
