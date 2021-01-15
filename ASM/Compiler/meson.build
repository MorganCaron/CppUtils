public_dependencies = [
	cppUtils_type_dep,
	cppUtils_asm_lexer_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([

])

headers = files([
	'include/CppUtils/ASM/Compiler/Bytecode.hpp',
	'include/CppUtils/ASM/Compiler/CompilationFunctions.hpp',
	'include/CppUtils/ASM/Compiler/Compiler.hpp',
	'include/CppUtils/ASM/Compiler/Context.hpp'
])

cppUtils_asm_compiler_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
