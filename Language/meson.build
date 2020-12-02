subdir('Parser')
subdir('Lexer')
subdir('Interpreter')
# subdir('Compiler')
subdir('ASM')

public_dependencies = [
	language_parser_dep,
	language_lexer_dep,
	language_interpreter_dep,
#	language_compiler_dep,
	language_asm_dep
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
