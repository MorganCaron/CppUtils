#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::ASM::VirtualMachine
{
	TEST_GROUP("Language/ASM/VirtualMachine")
	{
		using namespace std::literals;

		/*
		addTest("Nop", [] {
			static const auto virtualMachine = CppUtils::Language::ASM::VirtualMachine::VirtualMachine<std::uint64_t>{};
			auto context = CppUtils::Language::ASM::VirtualMachine::Context<std::uint64_t>{};
			virtualMachine.run(R"(
			nop
			)"sv, context);
		});

		addTest("Halt", [] {
			static const auto virtualMachine = CppUtils::Language::ASM::VirtualMachine::VirtualMachine<std::uint64_t>{};
			auto context = CppUtils::Language::ASM::VirtualMachine::Context<std::uint64_t>{};
			virtualMachine.run(R"(
			hlt
			)"sv, context);
		});

		addTest("Move", [] {
			static const auto virtualMachine = CppUtils::Language::ASM::VirtualMachine::VirtualMachine<std::uint64_t>{};
			auto context = CppUtils::Language::ASM::VirtualMachine::Context<std::uint64_t>{};
			virtualMachine.run(R"(
			mov r0, 20
			)"sv, context);
		});
		
		addTest("Add", [] {
			static const auto virtualMachine = CppUtils::Language::ASM::VirtualMachine::VirtualMachine<std::uint64_t>{};
			auto context = CppUtils::Language::ASM::VirtualMachine::Context<std::uint64_t>{};
			virtualMachine.run(R"(
			mov r0, 20
			add r0, 22
			)"sv, context);
		});
		*/
	}
}
