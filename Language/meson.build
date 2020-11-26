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

language_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
