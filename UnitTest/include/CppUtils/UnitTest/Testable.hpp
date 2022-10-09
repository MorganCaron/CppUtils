#pragma once

#include <vector>

#include <CppUtils/Type/Named.hpp>
#include <CppUtils/UnitTest/Test.hpp>
#include <CppUtils/UnitTest/TestRunner.hpp>

#define TEST_GROUP(name) \
	class TestGroup final: public CppUtils::UnitTest::Testable \
	{ \
	public: \
		TestGroup(): CppUtils::UnitTest::Testable(name) \
		{ \
			tests(); \
			CppUtils::UnitTest::TestRunner::instance().addTests(std::move(m_tests)); \
		} \
		void tests() final; \
	}; \
	inline auto testGroup = TestGroup{}; \
	void TestGroup::tests()

namespace CppUtils::UnitTest
{
	class Testable: public Type::Named
	{
	public:
		Testable(std::string name): Type::Named{std::move(name)}
		{}
		virtual ~Testable() = default;

		void addTest(std::string name, std::function<void()> function)
		{
			bool addSlash = !getName().empty() && !name.empty();
			name = std::string{getName()} + (addSlash ? "/" : "") + name;
			m_tests.emplace_back(std::move(name), function);
		}

		[[nodiscard]] const std::vector<Test>& getTests() const noexcept
		{
			return m_tests;
		}

		virtual void tests() = 0;
		
	protected:
		std::vector<Test> m_tests;
	};
}
