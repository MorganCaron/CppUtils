public_dependencies = [
	cppUtils_platform_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([
	'src/TextModifier.cpp'
])

headers = files([
	'include/CppUtils/Terminal/Terminal.hpp',
	'include/CppUtils/Terminal/TextStyle.hpp',
	'include/CppUtils/Terminal/TextColor.hpp',
	'include/CppUtils/Terminal/BackgroundColor.hpp',
	'include/CppUtils/Terminal/TextModifier.hpp'
])

cppUtils_terminal_lib = library(
	'CppUtils-Terminal',
	sources + headers,
	include_directories: includeDir,
	dependencies: public_dependencies + private_dependencies,
)

cppUtils_terminal_dep = declare_dependency(
	link_with: cppUtils_terminal_lib,
	include_directories: includeDir,
	dependencies: public_dependencies
)
