#pragma once

#include <cstdint>

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::ASM
{
	using namespace CppUtils::Language::ASM;

	const auto tests = std::vector<Test>{

		Test{"Language/ASM/VirtualMachine/HALT", [] {
			auto vm = VirtualMachine<std::uint32_t>{};
			auto context = Context{};
			const auto instructions = std::vector<Instruction>{
				Instruction{"HALT"_token}
			};
			
			vm.run(instructions, context);
		}},

		Test{"Language/ASM/VirtualMachine/NOP", [] {
			auto vm = VirtualMachine<std::uint32_t>{};
			auto context = Context{};
			const auto instructions = std::vector<Instruction>{
				Instruction{"NOP"_token},
				Instruction{"HALT"_token}
			};
			
			vm.run(instructions, context);
		}},

		/*
		Test{"Language/ASM/VirtualMachine/MOVE", [] {
			auto vm = VirtualMachine{};
			auto context = Context{};
			const auto instructions = std::vector<Instruction>{
				Instruction{"MOVE"_token, { "R0"_token, "42"_token }},
				Instruction{"HALT"_token}
			};
			
			vm.run(instructions, context);
		}},

		Test{"Language/ASM/VirtualMachine/PUSH and POP", [] {
			auto vm = VirtualMachine{};
			auto context = Context{};
			const auto instructions = std::vector<Instruction>{
				Instruction{"MOVE"_token, { "R0"_token, "42"_token }},
				Instruction{"PUSH"_token, { "R0"_token }},
				Instruction{"POP"_token, { "R1"_token }},
				Instruction{"HALT"_token}
			};
			
			vm.run(instructions, context);
		}}
		*/

	};
}
