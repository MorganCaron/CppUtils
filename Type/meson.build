public_dependencies = [
	hash_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([
	'src/TypeId.cpp'
])

headers = files([
	'include/Type/Named.hpp',
	'include/Type/Cloneable.hpp',
	'include/Type/NonCopyable.hpp',
	'include/Type/NonInstanciable.hpp',
	'include/Type/NonMovable.hpp',
	'include/Type/TypeId.hpp'
])

type_lib = library(
	'CppUtils-Type',
	sources + headers,
	include_directories: includeDir,
	dependencies: public_dependencies + private_dependencies,
)

type_dep = declare_dependency(
	link_with: type_lib,
	include_directories: includeDir,
	dependencies: public_dependencies
)
