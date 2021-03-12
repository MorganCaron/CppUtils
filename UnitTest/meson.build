public_dependencies = [
	cppUtils_parameters_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([
	
])

headers = files([
	'include/CppUtils/UnitTest/Test.hpp',
	'include/CppUtils/UnitTest/Testable.hpp',
	'include/CppUtils/UnitTest/TestException.hpp',
	'include/CppUtils/UnitTest/TestRunner.hpp',
	'include/CppUtils/UnitTest/TestSettingsLexer.hpp',
	'include/CppUtils/UnitTest/TestSettings.hpp'
])

cppUtils_unittest_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
