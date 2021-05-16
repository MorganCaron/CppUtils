subdir('Lexer')
subdir('Compiler')
subdir('VirtualMachine')

public_dependencies = [
	cppUtils_language_ir_lexer_dep,
	cppUtils_language_ir_compiler_dep,
	cppUtils_language_ir_virtualMachine_dep
]
private_dependencies = []

includeDir = include_directories(['.'])

sources = files([

])

headers = files([

])

cppUtils_language_ir_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
