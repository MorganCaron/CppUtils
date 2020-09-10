public_dependencies = [
	platform_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([
	
])

headers = files([
	'include/Terminal/TextModifier/TextStyle.hpp',
	'include/Terminal/TextModifier/TextColor.hpp',
	'include/Terminal/TextModifier/BackgroundColor.hpp'
])

terminal_textModifier_dep = declare_dependency(
	include_directories: includeDir,
	dependencies: public_dependencies
)
