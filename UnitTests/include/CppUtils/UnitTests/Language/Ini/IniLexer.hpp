#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Ini::IniLexer
{
	TEST_GROUP("Language/Ini/IniLexer")
	{
		using namespace CppUtils::Type::Literals;
		using namespace CppUtils::Language::Ini::Literals;
		using namespace CppUtils::Language::StringTree::Literals;

		addTest("minimalist", [] {
			
			const auto iniTree = R"(
			[Section 1]
			key=value
			)"_ini;
			CppUtils::Graph::logTreeNode(iniTree);

			const auto stringTree = R"(
			t"section" {
				t"Section 1" {
					t"key" {
						t"string" { "value" }
					}
				}
			}
			)"_stringTree;
			ASSERT(iniTree == stringTree);
		});

		addTest("comments", [] {
			const auto iniTree = R"(
			;comment
			[Section 1];comment
			key=value     ;comment
			;comment
			)"_ini;
			CppUtils::Graph::logTreeNode(iniTree);

			const auto stringTree = R"(
			t"section" {
				t"Section 1" {
					t"key" {
						t"string" { "value" }
					}
				}
			}
			)"_stringTree;
			ASSERT(iniTree == stringTree);
		});

		addTest("full", [] {
			const auto iniTree = R"(
			[Section 1]
			; comment
			Option 1 = value1 ; option 'Option 1' has value 'value1'
			oPtion 1 = value 2 ; option 'oPtion 1' has value ' value 2'

			[Numbers]
			num = -1285
			num_hex = 0x12ae

			float1 = -124.45667356
			float2 = +4.123E+4
			float3 = 412.3e4
			float4 = -1.1245864E-6

			inf1 = inf
			inf2 = -INFINITY

			[Booleans]
			bool1 = true
			bool2 = false
			)"_ini;
			CppUtils::Graph::logTreeNode(iniTree);

			const auto stringTree = R"(
			t"section" {
				t"Section 1" {
					t"Option 1" {
						t"string" { "value1" }
					}
					t"oPtion 1" {
						t"string" { "value 2" }
					}
				}
			}
			)"_stringTree;

			ASSERT(iniTree.childs.size() == 3);
			ASSERT(iniTree.childs.at(0) == stringTree.childs.at(0));
			ASSERT(iniTree.childs.at(1).at("Numbers"_token).childs.size() == 8);
			ASSERT(iniTree.childs.at(2).at("Booleans"_token).childs.size() == 2);
		});
	}
}
