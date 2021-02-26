subdir('Lexer')
subdir('Compiler')
subdir('VM')

public_dependencies = [
	cppUtils_language_asm_lexer_dep,
	cppUtils_language_asm_compiler_dep,
	cppUtils_language_asm_vm_dep
]
private_dependencies = []

includeDir = include_directories(['.'])

sources = files([

])

headers = files([

])

cppUtils_language_asm_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
