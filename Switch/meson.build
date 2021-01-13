public_dependencies = [
	cppUtils_type_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([
	'src/Switch.cpp'
])

headers = files([
	'include/CppUtils/Switch/Switch.hpp'
])

cppUtils_switch_lib = static_library(
	'CppUtils-Switch',
	sources + headers,
	include_directories: includeDir,
	dependencies: public_dependencies + private_dependencies,
)

cppUtils_switch_dep = declare_dependency(
	link_with: cppUtils_switch_lib,
	include_directories: includeDir,
	dependencies: public_dependencies
)
