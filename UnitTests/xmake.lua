target("CppUtils-UnitTests")
	set_kind("binary")
	add_deps("CppUtils")
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
	add_files("src/*.cpp")