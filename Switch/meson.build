public_dependencies = []
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([
	'src/Switch.cpp'
])

headers = files([
	'include/Switch/Switch.hpp'
])

switch_lib = library(
	'CppUtils-Switch',
	sources + headers,
	include_directories: includeDir,
	dependencies: public_dependencies + private_dependencies,
)

switch_dep = declare_dependency(
	link_with: switch_lib,
	include_directories: includeDir,
	dependencies: public_dependencies
)
