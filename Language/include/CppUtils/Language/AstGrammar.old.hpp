#pragma once

// #include <CppUtils/Language/VirtualMachine.old.hpp>

namespace CppUtils::Language::Grammar
{
	[[nodiscard]] auto getAstGrammar() -> Ast
	{
		using namespace Hashing::Literals;
		return Ast{"context", {
			AstNode{"source"_token, {
				AstNode{"LibSystem"_token, {
					AstNode{"allocateMemory"_token, { AstNode{reinterpret_cast<Hash::Token>(LibSystem::allocateMemory)} }},
					AstNode{"freeMemory"_token, { AstNode{reinterpret_cast<Hash::Token>(LibSystem::freeMemory)} }}
				}},
				AstNode{"parseToken"_token, {

				}},
				AstNode{"parseNode"_token, {
					
				}},
				AstNode{"main"_token, {
					AstNode{"move"_token, {
						AstNode{"token"_token, { AstNode{"register"_token, {
							AstNode{"token"_token, { AstNode{0} }}
						}} }},
						AstNode{"token"_token, { AstNode{"rodata"_token, {
							AstNode{"index"_token, { AstNode{0} }}
						}} }}
					}},
					AstNode{"cmp"_token, {
						AstNode{"deref"_token, {
							AstNode{"token"_token, { AstNode{"register"_token, {
								AstNode{"token"_token, { AstNode{0, {
									AstNode{"index"_token, { AstNode{0} }}
								}} }}
							}} }}
						}},
						AstNode{"value"_token, { AstNode{0} }}
					}},
					AstNode{"sete"_token, {
						AstNode{"token"_token, { AstNode{"register"_token, {
							AstNode{"token"_token, { AstNode{0} }}
						}} }}
					}},
					AstNode{"xor"_token, {
						AstNode{"token"_token, { AstNode{"register"_token, {
							AstNode{"token"_token, { AstNode{0} }}
						}} }},
						AstNode{"value"_token, { AstNode{1} }}
					}}
				}}
			}}
		}, VirtualMachine::getTokenNames()};
	}
}
