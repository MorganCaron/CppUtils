public_dependencies = [
	cppUtils_type_dep,
	cppUtils_terminal_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([
	'src/Logger.cpp'
])

headers = files([
	'include/CppUtils/Log/ChronoLogger.hpp',
	'include/CppUtils/Log/Logger.hpp'
])

cppUtils_log_lib = library(
	'CppUtils-Log',
	sources + headers,
	include_directories: includeDir,
	dependencies: public_dependencies + private_dependencies,
)

cppUtils_log_dep = declare_dependency(
	link_with: cppUtils_log_lib,
	include_directories: includeDir,
	dependencies: public_dependencies
)
