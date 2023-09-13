#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::VirtualMachine
{
	TEST_GROUP("Language/VirtualMachine")
	{
		using namespace std::literals;
		using namespace CppUtils::Hash::Literals;

		using CppUtils::Language::Ast;
		using CppUtils::Language::AstNode;

		addTest("empty source", [] {
			auto context = Ast{"context", {
				AstNode{"source"_token, {
					AstNode{"main"_token}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

			CppUtils::Language::VirtualMachine::run(context);
			context.log();

			const auto& result = CppUtils::Language::VirtualMachine::getReturnNode(context);
			EXPECT(result == AstNode{0});
		});

		addTest("move value", [] {
			auto context = Ast{"context", {
				AstNode{"source"_token, {
					AstNode{"main"_token, {
						AstNode{"move"_token, {
							AstNode{"token"_token, { AstNode{"register"_token, {
								AstNode{"token"_token, { AstNode{0} }}
							}} }},
							AstNode{"value"_token, { AstNode{3} }},
						}}
					}}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

			CppUtils::Language::VirtualMachine::run(context);
			context.log();

			const auto& result = CppUtils::Language::VirtualMachine::getReturnNode(context);
			EXPECT(result == AstNode{3});
		});

		addTest("move stack", [] {
			auto context = Ast{"context", {
				AstNode{"source"_token, {
					AstNode{"main"_token, {
						AstNode{"push"_token, {
							AstNode{"token"_token, { AstNode{"stack"_token} }},
							AstNode{"value"_token, { AstNode{3} }}
						}},
						AstNode{"move"_token, {
							AstNode{"token"_token, { AstNode{"register"_token, {
								AstNode{"token"_token, { AstNode{0} }}
							}} }},
							AstNode{"token"_token, { AstNode{"stack"_token, {
								AstNode{"reverseIndex"_token, { AstNode{0} }}
							}} }}
						}},
						AstNode{"pop"_token, {
							AstNode{"token"_token, { AstNode{"stack"_token} }}
						}}
					}}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

			CppUtils::Language::VirtualMachine::run(context);
			context.log();

			const auto& result = CppUtils::Language::VirtualMachine::getReturnNode(context);
			EXPECT(result == AstNode{3});
			const auto& stack = context.root["stack"_token];
			EXPECT(std::empty(stack.nodes));
		});

		addTest("deref rodata", [] {
			constexpr auto string = "A"sv;
			
			auto context = Ast{"context", {
				AstNode{"source"_token, {
					AstNode{"main"_token, {
						AstNode{"move"_token, {
							AstNode{"token"_token, { AstNode{"register"_token, {
								AstNode{"token"_token, { AstNode{0} }}
							}} }},
							AstNode{"deref"_token, {
								AstNode{"token"_token, { AstNode{"rodata"_token, {
									AstNode{"index"_token, { AstNode{0} }}
								}} }}
							}}
						}}
					}}
				}},
				AstNode{"rodata"_token, {
					AstNode{reinterpret_cast<std::uintptr_t>(std::data(string))}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

			CppUtils::Language::VirtualMachine::run(context);
			context.log();

			const auto& result = CppUtils::Language::VirtualMachine::getReturnNode(context);
			std::cout << result.value << std::endl;
			EXPECT(result == AstNode{'A'});
		});

		addTest("add value", [] {
			auto context = Ast{"context", {
				AstNode{"source"_token, {
					AstNode{"main"_token, {
						AstNode{"add"_token, {
							AstNode{"token"_token, { AstNode{"register"_token, {
								AstNode{"token"_token, { AstNode{0} }}
							}} }},
							AstNode{"value"_token, { AstNode{8} }},
						}}
					}}
				}},
				AstNode{"register"_token, {
					AstNode{0, { AstNode{3} }}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

			CppUtils::Language::VirtualMachine::run(context);
			context.log();

			const auto& result = CppUtils::Language::VirtualMachine::getReturnNode(context);
			EXPECT(result == AstNode{11});
		});

		addTest("xor", [] {
			auto context = Ast{"context", {
				AstNode{"source"_token, {
					AstNode{"main"_token, {
						AstNode{"xor"_token, {
							AstNode{"token"_token, { AstNode{"register"_token, {
								AstNode{"token"_token, { AstNode{0} }}
							}} }},
							AstNode{"token"_token, { AstNode{"register"_token, {
								AstNode{"token"_token, { AstNode{0, {
									AstNode{"index"_token, { AstNode{0} }}
								}} }}
							}} }}
						}}
					}}
				}},
				AstNode{"register"_token, {
					AstNode{0, { AstNode{1} }}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

			CppUtils::Language::VirtualMachine::run(context);
			context.log();

			const auto& result = CppUtils::Language::VirtualMachine::getReturnNode(context);
			EXPECT(result == AstNode{0});
		});

		addTest("cmp", [] {
			auto context = Ast{"context", {
				AstNode{"source"_token, {
					AstNode{"main"_token, {
						AstNode{"cmp"_token, {
							AstNode{"value"_token, { AstNode{1} }},
							AstNode{"value"_token, { AstNode{1} }}
						}},
						AstNode{"sete"_token, {
							AstNode{"token"_token, { AstNode{"register"_token, {
								AstNode{"token"_token, { AstNode{0} }}
							}} }}
						}}
					}}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

			CppUtils::Language::VirtualMachine::run(context);
			context.log();

			const auto& result = CppUtils::Language::VirtualMachine::getReturnNode(context);
			EXPECT(result == AstNode{1});
		});

		addTest("push", [] {
			auto context = Ast{"context", {
				AstNode{"source"_token, {
					AstNode{"main"_token, {
						AstNode{"push"_token, {
							AstNode{"token"_token, { AstNode{"stack"_token} }},
							AstNode{"value"_token, { AstNode{3} }}
						}}
					}}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

			CppUtils::Language::VirtualMachine::run(context);
			context.log();

			const auto& result = CppUtils::Language::VirtualMachine::getReturnNode(context);
			EXPECT(result == AstNode{0});
			const auto& stack = context.root["stack"_token];
			EXPECT(std::size(stack.nodes) == 1);
			EXPECT(stack.nodes[0].value == 3);
		});

		addTest("pop", [] {
			auto context = Ast{"context", {
				AstNode{"source"_token, {
					AstNode{"main"_token, {
						AstNode{"push"_token, {
							AstNode{"token"_token, { AstNode{"stack"_token} }},
							AstNode{"value"_token, { AstNode{3} }}
						}},
						AstNode{"pop"_token, {
							AstNode{"token"_token, { AstNode{"stack"_token} }}
						}}
					}}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

			CppUtils::Language::VirtualMachine::run(context);
			context.log();

			const auto& result = CppUtils::Language::VirtualMachine::getReturnNode(context);
			EXPECT(result == AstNode{0});
			const auto& stack = context.root["stack"_token];
			EXPECT(std::empty(stack.nodes));
		});

		addTest("call", [] {
			auto context = Ast{"context", {
				AstNode{"source"_token, {
					AstNode{"plus"_token, {
						AstNode{"move"_token, {
							AstNode{"token"_token, { AstNode{"register"_token, {
								AstNode{"token"_token, { AstNode{0} }}
							}} }},
							AstNode{"token"_token, { AstNode{"stack"_token, {
								AstNode{"reverseIndex"_token, { AstNode{0} }}
							}} }}
						}},
						AstNode{"add"_token, {
							AstNode{"token"_token, { AstNode{"register"_token, {
								AstNode{"token"_token, { AstNode{0} }}
							}} }},
							AstNode{"token"_token, { AstNode{"stack"_token, {
								AstNode{"reverseIndex"_token, { AstNode{1} }}
							}} }}
						}}
					}},
					AstNode{"main"_token, {
						AstNode{"push"_token, {
							AstNode{"token"_token, { AstNode{"stack"_token} }},
							AstNode{"value"_token, { AstNode{3} }}
						}},
						AstNode{"push"_token, {
							AstNode{"token"_token, { AstNode{"stack"_token} }},
							AstNode{"value"_token, { AstNode{8} }}
						}},
						AstNode{"call"_token, {
							AstNode{"token"_token, { AstNode{"plus"_token} }}
						}},
						AstNode{"pop"_token, {
							AstNode{"token"_token, { AstNode{"stack"_token} }}
						}},
						AstNode{"pop"_token, {
							AstNode{"token"_token, { AstNode{"stack"_token} }}
						}}
					}}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};
			context.tokenNames.insert({
				{ "plus"_token, "plus" }
			});

			CppUtils::Language::VirtualMachine::run(context);
			context.log();

			const auto& result = CppUtils::Language::VirtualMachine::getReturnNode(context);
			EXPECT(result == AstNode{11});
			const auto& stack = context.root["stack"_token];
			EXPECT(std::empty(stack.nodes));
		});

		addTest("externalCall allocateMemory", [] {
			auto context = Ast{"context", {
				AstNode{"source"_token, {
					AstNode{"LibSystem"_token, {
						AstNode{"allocateMemory"_token, { AstNode{reinterpret_cast<CppUtils::Hash::Token>(CppUtils::Language::LibSystem::allocateMemory)} }},
						AstNode{"freeMemory"_token, { AstNode{reinterpret_cast<CppUtils::Hash::Token>(CppUtils::Language::LibSystem::freeMemory)} }}
					}},
					AstNode{"main"_token, {
						AstNode{"push"_token, {
							AstNode{"token"_token, { AstNode{"stack"_token} }},
							AstNode{"value"_token, { AstNode{5} }}
						}},
						AstNode{"externalCall"_token, {
							AstNode{"token"_token, { AstNode{"LibSystem"_token, {
								AstNode{"token"_token, { AstNode{"allocateMemory"_token} }}
							}} }}
						}},
						AstNode{"pop"_token, {
							AstNode{"token"_token, { AstNode{"stack"_token} }}
						}}/*,
						AstNode{"push"_token, {
							AstNode{"token"_token, { AstNode{"stack"_token} }},
							AstNode{"token"_token, { AstNode{"register"_token, {
								AstNode{"token"_token, { AstNode{"R0"_token, {
									AstNode{"index"_token, { AstNode{0} }}
								}} }}
							}} }}
						}},
						AstNode{"externalCall"_token, {
							AstNode{"token"_token, { AstNode{"LibSystem"_token, {
								AstNode{"token"_token, { AstNode{"freeMemory"_token} }}
							}} }}
						}},
						AstNode{"pop"_token, {
							AstNode{"token"_token, { AstNode{"stack"_token} }}
						}}*/
					}}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

			CppUtils::Language::VirtualMachine::run(context);
			context.log();

			const auto& result = CppUtils::Language::VirtualMachine::getReturnNode(context);
			const auto* pointer = reinterpret_cast<const char*>(result.value);
			std::cout << *pointer << std::endl;
			EXPECT(sizeof(pointer) == 5);
			const auto& stack = context.root["stack"_token];
			EXPECT(std::empty(stack.nodes));
		});
	}
}
