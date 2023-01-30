#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Parser::Grammar::AstGrammar
{
	TEST_GROUP("Language/Parser/Grammar/AstGrammar")
	{
		using namespace std::literals;
		using namespace CppUtils::Hash::Literals;

		using CppUtils::Language::Ast;
		using CppUtils::Language::AstNode;

		addTest("empty grammar", [] {
			constexpr auto source = ""sv;

			auto context = Ast{"context", {
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
				}},
				AstNode{"rodata"_token, {
					AstNode{reinterpret_cast<std::uintptr_t>(std::data(source))}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

            const auto result = CppUtils::Language::VirtualMachine::run(context);
			context.log();

			TEST_ASSERT(result == AstNode{0});
		});

		addTest("char", [] {
			constexpr auto source = "c"sv;

			auto context = Ast{"context", {
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
								AstNode{'c'}
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
				}},
				AstNode{"rodata"_token, {
					AstNode{reinterpret_cast<std::uintptr_t>(std::data(source))}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

            const auto result = CppUtils::Language::VirtualMachine::run(context);
			context.log();

			TEST_ASSERT(result == AstNode{0});
		});
	}
}
