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
					AstNode{"main"_token, {
						AstNode{"return"_token}
					}}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

            const auto result = CppUtils::Language::VirtualMachine::run(context);
			context.log();

			TEST_ASSERT(result == AstNode{0});
		});

		addTest("move value", [] {
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
							AstNode{"value"_token, { AstNode{3} }},
						}},
						AstNode{"return"_token}
					}}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

            const auto result = CppUtils::Language::VirtualMachine::run(context);
			context.log();

			TEST_ASSERT(result == AstNode{3});
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
							AstNode{"token"_token, {
								AstNode{"register"_token, {
									AstNode{"token"_token, {
										AstNode{"R0"_token}
									}}
								}}
							}},
							AstNode{"token"_token, {
								AstNode{"stack"_token, {
									AstNode{"reverseIndex"_token, {
										AstNode{0}
									}}
								}}
							}}
						}},
						AstNode{"pop"_token, {
							AstNode{"token"_token, { AstNode{"stack"_token} }}
						}},
						AstNode{"return"_token}
					}}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

            const auto result = CppUtils::Language::VirtualMachine::run(context);
			context.log();

			TEST_ASSERT(result == AstNode{3});
			const auto& stack = context.root["stack"_token];
			TEST_ASSERT(std::empty(stack.nodes));
		});

		addTest("deref rodata", [] {
			constexpr auto string = "A"sv;
			
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
							AstNode{"deref"_token, {
								AstNode{"token"_token, {
									AstNode{"rodata"_token, {
										AstNode{"index"_token, {
											AstNode{0}
										}}
									}}
								}}
							}}
						}},
						AstNode{"return"_token}
					}}
				}},
				AstNode{"rodata"_token, {
					AstNode{reinterpret_cast<std::uintptr_t>(std::data(string))}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

            const auto result = CppUtils::Language::VirtualMachine::run(context);
			context.log();

			TEST_ASSERT(result == AstNode{'A'});
		});

		addTest("add value", [] {
			auto context = Ast{"context", {
				AstNode{"source"_token, {
					AstNode{"main"_token, {
						AstNode{"add"_token, {
							AstNode{"token"_token, {
								AstNode{"register"_token, {
									AstNode{"token"_token, {
										AstNode{"R0"_token}
									}}
								}}
							}},
							AstNode{"value"_token, { AstNode{8} }},
						}},
						AstNode{"return"_token}
					}}
				}},
				AstNode{"register"_token, {
					AstNode{"R0"_token, {
						AstNode{3}
					}}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

            const auto result = CppUtils::Language::VirtualMachine::run(context);
			context.log();

			TEST_ASSERT(result == AstNode{11});
		});

		addTest("xor", [] {
			auto context = Ast{"context", {
				AstNode{"source"_token, {
					AstNode{"main"_token, {
						AstNode{"xor"_token, {
							AstNode{"token"_token, {
								AstNode{"register"_token, {
									AstNode{"token"_token, {
										AstNode{"R0"_token}
									}}
								}}
							}},
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
						AstNode{"return"_token}
					}}
				}},
				AstNode{"register"_token, {
					AstNode{"R0"_token, {
						AstNode{1}
					}}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

            const auto result = CppUtils::Language::VirtualMachine::run(context);
			context.log();

			TEST_ASSERT(result == AstNode{0});
		});

		addTest("cmp", [] {
			auto context = Ast{"context", {
				AstNode{"source"_token, {
					AstNode{"main"_token, {
						AstNode{"cmp"_token, {
							AstNode{"value"_token, {
								AstNode{1}
							}},
							AstNode{"value"_token, {
								AstNode{1}
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
						AstNode{"return"_token}
					}}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

            const auto result = CppUtils::Language::VirtualMachine::run(context);
			context.log();

			TEST_ASSERT(result == AstNode{1});
		});

		addTest("push", [] {
			auto context = Ast{"context", {
				AstNode{"source"_token, {
					AstNode{"main"_token, {
						AstNode{"push"_token, {
							AstNode{"token"_token, { AstNode{"stack"_token} }},
							AstNode{"value"_token, { AstNode{3} }}
						}},
						AstNode{"return"_token}
					}}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

            const auto result = CppUtils::Language::VirtualMachine::run(context);
			context.log();

			TEST_ASSERT(result == AstNode{0});
			const auto& stack = context.root["stack"_token];
			TEST_ASSERT(std::size(stack.nodes) == 1);
			TEST_ASSERT(stack.nodes[0].value == 3);
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
						}},
						AstNode{"return"_token}
					}}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};

            const auto result = CppUtils::Language::VirtualMachine::run(context);
			context.log();

			TEST_ASSERT(result == AstNode{0});
			const auto& stack = context.root["stack"_token];
			TEST_ASSERT(std::empty(stack.nodes));
		});

		addTest("call", [] {
			auto context = Ast{"context", {
				AstNode{"source"_token, {
					AstNode{"plus"_token, {
						AstNode{"move"_token, {
							AstNode{"token"_token, {
								AstNode{"register"_token, {
									AstNode{"token"_token, {
										AstNode{"R0"_token}
									}}
								}}
							}},
							AstNode{"token"_token, {
								AstNode{"stack"_token, {
									AstNode{"reverseIndex"_token, {
										AstNode{0, {
											AstNode{"index"_token, {
												AstNode{0, {
													AstNode{"index"_token, {
														AstNode{0}
													}}
												}}
											}}
										}}
									}}
								}}
							}}
						}},
						AstNode{"add"_token, {
							AstNode{"token"_token, {
								AstNode{"register"_token, {
									AstNode{"token"_token, {
										AstNode{"R0"_token}
									}}
								}}
							}},
							AstNode{"token"_token, {
								AstNode{"stack"_token, {
									AstNode{"reverseIndex"_token, {
										AstNode{0, {
											AstNode{"index"_token, {
												AstNode{1, {
													AstNode{"index"_token, {
														AstNode{0}
													}}
												}}
											}}
										}}
									}}
								}}
							}}
						}},
						AstNode{"return"_token}
					}},
					AstNode{"main"_token, {
						AstNode{"push"_token, {
							AstNode{"token"_token, {
								AstNode{"stack"_token}
							}},
							AstNode{"value"_token, {
								AstNode{"plus"_token, {
									AstNode{"value"_token, { AstNode{3} }},
									AstNode{"value"_token, { AstNode{8} }}
								}}
							}}
						}},
						AstNode{"call"_token},
						AstNode{"pop"_token, {
							AstNode{"token"_token, { AstNode{"stack"_token} }}
						}},
						AstNode{"return"_token}
					}}
				}}
			}, CppUtils::Language::VirtualMachine::getTokenNames()};
			context.tokenNames.insert({
				{ "plus"_token, "plus" }
			});

            const auto result = CppUtils::Language::VirtualMachine::run(context);
			context.log();

			TEST_ASSERT(result == AstNode{11});
		});
	}
}
