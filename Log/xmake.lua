target("CppUtils-Log")
	set_kind("$(kind)")
	add_deps(
		"CppUtils-System",
		"CppUtils-Type",
		"CppUtils-String",
		"CppUtils-Terminal"
	)
	set_policy("build.merge_archive", true)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
