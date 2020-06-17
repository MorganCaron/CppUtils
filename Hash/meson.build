public_dependencies = []
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([
	
])

headers = files([
	'include/Hash/Hash.hpp'
])

hash_lib = library(
	'CppUtils-Hash',
	sources + headers,
	include_directories: includeDir,
	dependencies: public_dependencies + private_dependencies
)

hash_dep = declare_dependency(
	link_with: hash_lib,
	include_directories: includeDir,
	dependencies: public_dependencies
)
