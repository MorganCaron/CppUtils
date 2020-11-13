public_dependencies = [
	external_dep,
	hash_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([
	'src/TypeId.cpp'
])

headers = files([
	'include/CppUtils/Type/Named.hpp',
	'include/CppUtils/Type/Cloneable.hpp',
	'include/CppUtils/Type/NonCopyable.hpp',
	'include/CppUtils/Type/NonInstanciable.hpp',
	'include/CppUtils/Type/NonMovable.hpp',
	'include/CppUtils/Type/Typed.hpp',
	'include/CppUtils/Type/TypeId.hpp'
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
