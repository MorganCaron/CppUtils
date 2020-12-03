public_dependencies = [
	cppUtils_platform_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([
	
])

headers = files([
	'include/CppUtils/Terminal/TextModifier/TextStyle.hpp',
	'include/CppUtils/Terminal/TextModifier/TextColor.hpp',
	'include/CppUtils/Terminal/TextModifier/BackgroundColor.hpp',
	'include/CppUtils/Terminal/TextModifier/TextModifier.hpp'
])

cppUtils_terminal_textModifier_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
