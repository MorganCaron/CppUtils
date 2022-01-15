target("CppUtils-Log")
	set_kind("$(kind)")
	set_policy("build.merge_archive", true)
	add_deps(
		"CppUtils-External",
		"CppUtils-Type",
		"CppUtils-Terminal"
	)
	add_defines("DLL_EXPORT")
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
	add_files("src/*.cpp")
