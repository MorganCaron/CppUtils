target("CppUtils-Terminal")
	set_kind("$(kind)")
	add_deps(
		"CppUtils-External",
		"CppUtils-Platform",
		"CppUtils-Container"
	)
	set_policy("build.merge_archive", true)
	add_defines("DLL_EXPORT")
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
	add_files("src/*.cpp")
