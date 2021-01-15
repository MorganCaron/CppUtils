subdir('Lexer')
subdir('Compiler')
subdir('VM')

public_dependencies = [
	cppUtils_asm_lexer_dep,
	cppUtils_asm_compiler_dep,
	cppUtils_asm_vm_dep
]
private_dependencies = []

includeDir = include_directories(['.'])

sources = files([

])

headers = files([

])

cppUtils_asm_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
