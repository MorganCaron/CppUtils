#pragma once

#include <array>

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::ASM::VirtualMachine
{
	TEST_GROUP("Language/ASM/VirtualMachine")
	{
		using namespace std::literals;

		/*
		addTest("Nop", [] {
			static const auto vm = CppUtils::Language::ASM::VM::VirtualMachine<std::int64_t>{};
			auto context = CppUtils::Language::ASM::VM::Context<std::int64_t>{};
			vm.run(R"(
			nop
			)"sv, context);
		});

		addTest("Halt", [] {
			static const auto vm = CppUtils::Language::ASM::VM::VirtualMachine<std::int64_t>{};
			auto context = CppUtils::Language::ASM::VM::Context<std::int64_t>{};
			vm.run(R"(
			hlt
			)"sv, context);
		});

		addTest("Move", [] {
			static const auto vm = CppUtils::Language::ASM::VM::VirtualMachine<std::int64_t>{};
			auto context = CppUtils::Language::ASM::VM::Context<std::int64_t>{};
			vm.run(R"(
			mov r0, 20
			)"sv, context);
		});
		
		addTest("Add", [] {
			static const auto vm = CppUtils::Language::ASM::VM::VirtualMachine<std::int64_t>{};
			auto context = CppUtils::Language::ASM::VM::Context<std::int64_t>{};
			vm.run(R"(
			mov r0, 20
			add r0, 22
			)"sv, context);
		});
		*/
	}
}
