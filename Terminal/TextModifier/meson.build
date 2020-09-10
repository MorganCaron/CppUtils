public_dependencies = [
	platform_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([
	
])

headers = files([
	'include/Terminal/TextModifier/TextStyle.hpp',
	'include/Terminal/TextModifier/TextColor.hpp',
	'include/Terminal/TextModifier/BackgroundColor.hpp'
])

terminal_textModifier_lib = library(
	'CppUtils-Terminal-TextModifier',
	sources + headers,
	include_directories: includeDir,
	dependencies: public_dependencies + private_dependencies
)

terminal_textModifier_dep = declare_dependency(
	link_with: terminal_textModifier_lib,
	include_directories: includeDir,
	dependencies: public_dependencies
)
