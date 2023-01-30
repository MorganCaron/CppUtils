#pragma once

#include <CppUtils/Language/VirtualMachine.hpp>

namespace CppUtils::Language::Grammar
{
	[[nodiscard]] auto getAstGrammar() -> Ast
	{
		using namespace Hash::Literals;
		return Ast{"context", {
			AstNode{"source"_token, {
				AstNode{"main"_token, {
					AstNode{"move"_token, {
						AstNode{"token"_token, {
							AstNode{"register"_token, {
								AstNode{"token"_token, {
									AstNode{"R0"_token}
								}}
							}}
						}},
						AstNode{"token"_token, {
							AstNode{"rodata"_token, {
								AstNode{"index"_token, {
									AstNode{0}
								}}
							}}
						}}
					}},
					AstNode{"cmp"_token, {
						AstNode{"deref"_token, {
							AstNode{"token"_token, {
								AstNode{"register"_token, {
									AstNode{"token"_token, {
										AstNode{"R0"_token, {
											AstNode{"index"_token, {
												AstNode{0}
											}}
										}}
									}}
								}}
							}}
						}},
						AstNode{"value"_token, {
							AstNode{0}
						}}
					}},
					AstNode{"sete"_token, {
						AstNode{"token"_token, {
							AstNode{"register"_token, {
								AstNode{"token"_token, {
									AstNode{"R0"_token}
								}}
							}}
						}}
					}},
					AstNode{"xor"_token, {
						AstNode{"token"_token, {
							AstNode{"register"_token, {
								AstNode{"token"_token, {
									AstNode{"R0"_token}
								}}
							}}
						}},
						AstNode{"value"_token, {
							AstNode{1}
						}}
					}},
					AstNode{"return"_token}
				}}
			}}
		}, VirtualMachine::getTokenNames()};
	}
}
