subdir('Parser')
subdir('Lexer')
subdir('VM')
# subdir('Compiler')

public_dependencies = [
	language_parser_dep,
	language_lexer_dep,
	language_vm_dep,
#	language_compiler_dep
]
private_dependencies = []

includeDir = include_directories(['.'])

sources = files([

])

headers = files([

])

language_lib = library(
	'CppUtils-Language',
	sources + headers,
	include_directories: includeDir,
	dependencies: public_dependencies + private_dependencies,
)

language_dep = declare_dependency(
	link_with: language_lib,
	include_directories: includeDir,
	dependencies: public_dependencies
)
