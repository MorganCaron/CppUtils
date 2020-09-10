subdir('TextModifier')

public_dependencies = [
	terminal_textModifier_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([
	
])

headers = files([
	'include/Terminal/Terminal.hpp',
	'include/Terminal/Parameters.hpp',
	'include/Terminal/TextModifier.hpp'
])

terminal_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
