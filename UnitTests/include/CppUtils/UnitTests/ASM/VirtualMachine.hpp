#pragma once

#include <array>

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::ASM::VirtualMachine
{
	const auto tests = std::vector<Test>{

		Test{"ASM/VirtualMachine/Halt", [] {
			using namespace std::literals;
			CppUtils::ASM::VM::VirtualMachine<std::int64_t>::run(R"(
			hlt
			)"sv);
		}},

		Test{"ASM/VirtualMachine/Nop", [] {
			using namespace std::literals;
			CppUtils::ASM::VM::VirtualMachine<std::int64_t>::run(R"(
			nop
			)"sv);
		}},

		Test{"ASM/VirtualMachine/Move", [] {
			using namespace std::literals;
			CppUtils::ASM::VM::VirtualMachine<std::int64_t>::run(R"(
			mov r0, 20
			)"sv);
		}},
		
		Test{"ASM/VirtualMachine/Add", [] {
			using namespace std::literals;
			CppUtils::ASM::VM::VirtualMachine<std::int64_t>::run(R"(
			mov r0, 20
			add r0, 22
			)"sv);
		}}/*,

		Test{"ASM/VirtualMachine/PUSH and POP", [] {
			auto vm = VirtualMachine<std::uint32_t>{};
			auto context = Context{};
			const auto instructions = std::vector<Instruction>{
				Instruction{"MOVE"_token, { "R0"_token, "42"_token }},
				Instruction{"PUSH"_token, { "R0"_token }},
				Instruction{"POP"_token, { "R1"_token }},
				Instruction{"HALT"_token}
			};
			
			vm.run(instructions, context);
		}}*/

	};
}
