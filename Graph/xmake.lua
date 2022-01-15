target("CppUtils-Graph")
	set_kind("$(kind)")
	set_policy("build.merge_archive", true)
	add_deps(
		"CppUtils-Type",
		"CppUtils-Log"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
