includeDir = include_directories(['include/', '.'])

sources = files([
	'src/main.cpp'
])

headers = files([
	'include/UnitTests/Container/MeshNode.hpp',
	'include/UnitTests/Container/Vector.hpp',
	'include/UnitTests/FileSystem/File/Binary.hpp',
	'include/UnitTests/FileSystem/File/String.hpp',
	'include/UnitTests/Log/Logger.hpp',
	'include/UnitTests/String/String.hpp',
	'include/UnitTests/Terminal/Parameters.hpp',
	'include/UnitTests/Test/UnitTest.hpp',
	'include/UnitTests/Type/Index.hpp'
])

unitTests_exe = executable(
	'UnitTests',
	sources: sources + headers,
	include_directories: includeDir,
	dependencies: [cppUtils_dep]
)

test('Unit tests', unitTests_exe)
