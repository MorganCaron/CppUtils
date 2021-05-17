public_dependencies = [
	cppUtils_log_dep,
	cppUtils_type_dep,
	cppUtils_language_parser_dep,
	cppUtils_language_compiler_dep,
	cppUtils_language_asm_lexer_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([

])

headers = files([
	'include/CppUtils/Language/ASM/Compiler/Bytecode.hpp',
	'include/CppUtils/Language/ASM/Compiler/CompilationFunctions.hpp',
	'include/CppUtils/Language/ASM/Compiler/Compiler.hpp',
	'include/CppUtils/Language/ASM/Compiler/Context.hpp',
	'include/CppUtils/Language/ASM/Compiler/Output.hpp'
])

cppUtils_language_asm_compiler_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
