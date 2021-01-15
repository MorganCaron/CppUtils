subdir('Parser')
subdir('Lexer')
subdir('IR')
subdir('VM')
subdir('Compiler')

public_dependencies = [
	cppUtils_language_parser_dep,
	cppUtils_language_lexer_dep,
	cppUtils_language_ir_dep,
	cppUtils_language_vm_dep,
	cppUtils_language_compiler_dep
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
