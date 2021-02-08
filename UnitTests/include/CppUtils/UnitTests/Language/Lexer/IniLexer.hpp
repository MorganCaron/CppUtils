#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Lexer::IniLexer
{
	const auto tests = std::vector<CppUtils::Test>{

		CppUtils::Test{"Language/Lexer/IniLexer/minimalist", [] {
			using namespace CppUtils::Language::Lexer::Literals;
			const auto iniTree = R"(
			[Section 1]
			key=value
			)"_ini;
			CppUtils::Graph::logTreeNode(iniTree);

			const auto stringTree = R"(
			"section" {
				"Section 1" {
					"key" {
						"string" { "value" }
					}
				}
			}
			)"_stringTree;
			ASSERT(iniTree == stringTree);
		}},

		CppUtils::Test{"Language/Lexer/IniLexer/comments", [] {
			using namespace CppUtils::Language::Lexer::Literals;
			const auto iniTree = R"(
			;comment
			[Section 1];comment
			key=value     ;comment
			;comment
			)"_ini;
			CppUtils::Graph::logTreeNode(iniTree);

			const auto stringTree = R"(
			"section" {
				"Section 1" {
					"key" {
						"string" { "value" }
					}
				}
			}
			)"_stringTree;
			ASSERT(iniTree == stringTree);
		}},

		CppUtils::Test{"Language/Lexer/IniLexer/full", [] {
			using namespace CppUtils::Language::Lexer::Literals;
			const auto iniTree = R"(
			[Section 1]
			; comment
			Option 1 = value 1 ; option 'Option 1' has value 'value 1'
			oPtion 1 = \ value2\ \ \ ; option 'oPtion 1' has value ' value 2   ', 'oPtion 1' and 'Option 1' are different

			[Numbers]
			num = -1285
			num_bin = 0b01101001
			num_hex = 0x12ae,0xAc2B
			num_oct = 01754

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
			"section" {
				"Section 1" {
					"Option 1" {
						"string" { "value 1" }
					}
					"oPtion 1" {
						"string" { " value 2   " }
					}
				}
			}
			"section" {
				"Numbers" {
					"num" { "-1285" }
					"num_bin" { "0b01101001" }
					"num_hex" { "0x12ae,0xAc2B" }
					"num_oct" { "01754" }
					"float1" { "-124.45667356" }
					"float2" { "+4.123E+4" }
					"float3" { "412.3e4" }
					"float4" { "-1.1245864E-6" }
					"inf1" { "inf" }
					"inf2" { "-inf" }
				}
				"Booleans" {
					"bool1" { "true" }
					"bool2" { "false" }
				}
			}
			)"_stringTree;
			ASSERT(iniTree == stringTree);
		}}

	};
}
