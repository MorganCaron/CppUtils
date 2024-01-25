#pragma once

#include <CppUtils.hpp>

namespace CppUtils::UnitTests::Language::Ini::IniLexer
{
	TEST_GROUP("Language/Ini/IniLexer")
	{
		using namespace CppUtils::Type::Literals;
		using namespace CppUtils::Language::Ini::Literals;
		using namespace CppUtils::Language::Json::Literals;

		addTest("Minimalist", [] {
			const auto iniTree = R"(
			[Section 1]
			key=value
			)"_ini;
			CppUtils::Log::TreeNodeLogger::log(iniTree);

			const auto jsonTree = R"(
			{
				"section": {
					"Section 1": {
						"key": {
							"string": "value"
						}
					}
				}
			}
			)"_json;
			EXPECT(iniTree == jsonTree);
		});

		addTest("Number", [] {
			const auto iniTree = R"(
			[Numbers]
			pi=3.14
			)"_ini;
			CppUtils::Log::TreeNodeLogger::log(iniTree);

			const auto jsonTree = R"(
			{
				"section": {
					"Numbers": {
						"pi": {
							"float": 3.14
						}
					}
				}
			}
			)"_json;
			EXPECT(iniTree == jsonTree);
		});

		addTest("Comments", [] {
			const auto iniTree = R"(
			;comment
			[Section 1];comment
			key=value     ;comment
			;comment
			)"_ini;
			CppUtils::Log::TreeNodeLogger::log(iniTree);

			const auto jsonTree = R"(
			{
				"section": {
					"Section 1": {
						"key": {
							"string": "value"
						}
					}
				}
			}
			)"_json;
			EXPECT(iniTree == jsonTree);
		});

		addTest("Full", [] {
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
			CppUtils::Log::TreeNodeLogger::log(iniTree);

			const auto jsonTree = R"(
			{
				"section": {
					"Section 1": {
						"Option 1": {
							"string": "value1"
						},
						"oPtion 1": {
							"string": "value 2"
						}
					}
				}
			}
			)"_json;
			EXPECT(iniTree.childs.size() == 3);
			EXPECT(iniTree.childs[0] == jsonTree.childs.at(0));
			EXPECT(iniTree.childs[1].at("Numbers"_token).childs.size() == 8);
			EXPECT(iniTree.childs[2].at("Booleans"_token).childs.size() == 2);
		});
	}
}
