subdir('Parser')
subdir('Lexer')
subdir('VirtualMachine')
subdir('Compiler')
subdir('IR')
subdir('Ini')
subdir('Json')
subdir('Markdown')
subdir('StringTree')
subdir('ASM')
subdir('Xml')

public_dependencies = [
	cppUtils_language_parser_dep,
	cppUtils_language_lexer_dep,
	cppUtils_language_virtualMachine_dep,
	cppUtils_language_compiler_dep,
	cppUtils_language_ir_dep,
	cppUtils_language_ini_dep,
	cppUtils_language_json_dep,
	cppUtils_language_markdown_dep,
	cppUtils_language_stringtree_dep,
	cppUtils_language_asm_dep,
	cppUtils_language_xml_dep
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
