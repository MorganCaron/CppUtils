public_dependencies = []
private_dependencies = [
	cppUtils_dep
]

includeDir = include_directories(['include/', '.'])

sources = files([
	'src/main.cpp'
])

headers = files([
	'include/UnitTests/Chrono/Chronometer.hpp',
	'include/UnitTests/Container/MeshNode.hpp',
	'include/UnitTests/Container/Vector.hpp',
	'include/UnitTests/FileSystem/File/Binary.hpp',
	'include/UnitTests/FileSystem/File/String.hpp',
	'include/UnitTests/Log/ChronoLogger.hpp',
	'include/UnitTests/Log/Logger.hpp',
	'include/UnitTests/String/String.hpp',
	'include/UnitTests/Switch/Switch.hpp',
	'include/UnitTests/Terminal/Parameters.hpp',
	'include/UnitTests/Test/UnitTest.hpp',
	'include/UnitTests/Type/Index.hpp'
])

unitTests_exe = executable(
	'UnitTests',
	sources: sources + headers,
	include_directories: includeDir,
	dependencies: private_dependencies
)

test('Unit tests', unitTests_exe)
