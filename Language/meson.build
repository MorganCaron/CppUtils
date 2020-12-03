subdir('Parser')
subdir('Lexer')
subdir('Interpreter')
# subdir('Compiler')
subdir('ASM')

public_dependencies = [
	cppUtils_language_parser_dep,
	cppUtils_language_lexer_dep,
	cppUtils_language_interpreter_dep,
#	cppUtils_language_compiler_dep,
	cppUtils_language_asm_dep
]
private_dependencies = []

includeDir = include_directories(['.'])

sources = files([

])

headers = files([

])

cppUtils_language_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
