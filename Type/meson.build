public_dependencies = [
	cppUtils_external_dep,
	cppUtils_hash_dep
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
	'include/CppUtils/Type/NonInstantiable.hpp',
	'include/CppUtils/Type/NonMovable.hpp',
	'include/CppUtils/Type/Typed.hpp',
	'include/CppUtils/Type/TypeId.hpp'
])

cppUtils_type_lib = library(
	'CppUtils-Type',
	sources + headers,
	include_directories: includeDir,
	dependencies: public_dependencies + private_dependencies,
)

cppUtils_type_dep = declare_dependency(
	link_with: cppUtils_type_lib,
	include_directories: includeDir,
	dependencies: public_dependencies
)
