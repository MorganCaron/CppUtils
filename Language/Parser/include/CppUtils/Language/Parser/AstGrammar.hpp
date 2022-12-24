#pragma once

#include <CppUtils/Language/Parser/Ast.hpp>

namespace CppUtils::Language::Parser::Grammar
{
	using namespace Hash::Literals;

	const auto astGrammar = Ast{"astGrammar"_token, {
        AstNode{"main"_token, { // main: node eof;
            AstNode{"set"_token, { AstNode{"node"_token} }},
            AstNode{"call"_token},
            AstNode{"eof"_token},
            AstNode{"return"_token}
        }},
        AstNode{"node"_token, { // node: name '{' nodes '}';
            AstNode{"set"_token, { AstNode{"name"_token} }},
            AstNode{"call"_token},
            AstNode{"returnIfNot"_token},
            AstNode{"set"_token, { AstNode{'{'} }},
            AstNode{"push"_token},
            AstNode{"read"_token},
            AstNode{"compare"_token},
            AstNode{"drop"_token},
            AstNode{"set"_token, { AstNode{"nodes"_token} }},
            AstNode{"call"_token},
            AstNode{"returnIfNot"_token},
            AstNode{"push"_token},
            AstNode{"set"_token, { AstNode{'}'} }},
            AstNode{"read"_token},
            AstNode{"compare"_token},
            AstNode{"drop"_token},
            AstNode{"return"_token}
        }},
        AstNode{"nodes"_token, { // nodes: node ~nodes;
            AstNode{"set"_token, { AstNode{"node"_token} }},
            AstNode{"call"_token},
            AstNode{"returnIfNot"_token},
            AstNode{"set"_token, { AstNode{"nodes"_token} }},
            AstNode{"call"_token},
            AstNode{"return"_token}
        }},
        AstNode{"name"_token, { // name: (token || value);
            AstNode{"set"_token, { AstNode{"token"_token} }},
            AstNode{"call"_token},
            AstNode{"returnIf"_token},
            AstNode{"set"_token, { AstNode{"value"_token} }},
            AstNode{"call"_token},
            AstNode{"return"_token}
        }},
        AstNode{"token"_token, { // token: read !is('_') token;
            AstNode{"set"_token, { AstNode{"read"_token} }},
            AstNode{"hash"_token},
            AstNode{"return"_token}
        }},
        AstNode{"value"_token, { AstNode{"return"_token} }},  // value: ;
        AstNode{"spaces"_token, { // spaces: space ~spaces;
            AstNode{"set"_token, { AstNode{"space"_token} }},
            AstNode{"call"_token},
            AstNode{"returnIfNot"_token},
            AstNode{"set"_token, { AstNode{"spaces"_token} }},
            AstNode{"call"_token},
            AstNode{"return"_token}
        }},
        AstNode{"space"_token, { // space: ' ' || '\n' || '\t';
            AstNode{"set"_token, { AstNode{' '} }},
            AstNode{"push"_token},
            AstNode{"read"_token},
            AstNode{"compare"_token},
            AstNode{"drop"_token},
            AstNode{"returnIf"_token},
            AstNode{"set"_token, { AstNode{'\n'} }},
            AstNode{"push"_token},
            AstNode{"read"_token},
            AstNode{"compare"_token},
            AstNode{"drop"_token},
            AstNode{"returnIf"_token},
            AstNode{"set"_token, { AstNode{'\t'} }},
            AstNode{"push"_token},
            AstNode{"read"_token},
            AstNode{"compare"_token},
            AstNode{"drop"_token},
            AstNode{"return"_token}
        }},
        AstNode{"read"_token, { // read: read+;
            AstNode{"read"_token},
            AstNode{"increment"_token},
            AstNode{"return"_token}
        }}
    }};
}
