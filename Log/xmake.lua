target("CppUtils-Log")
	set_kind("static")
	set_policy("build.merge_archive", true)
	add_deps(
		"CppUtils-Type",
		"CppUtils-Terminal"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
	add_files("src/*.cpp")