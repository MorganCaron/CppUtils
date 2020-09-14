public_dependencies = [
	terminal_dep,
	switch_dep
]
private_dependencies = []

includeDir = include_directories(['include/', '.'])

sources = files([
	'src/Logger.cpp'
])

headers = files([
	'include/Log/ChronoLogger.hpp',
	'include/Log/Logger.hpp'
])

log_lib = library(
	'CppUtils-Log',
	sources + headers,
	include_directories: includeDir,
	dependencies: public_dependencies + private_dependencies,
)

log_dep = declare_dependency(
	link_with: log_lib,
	include_directories: includeDir,
	dependencies: public_dependencies
)
