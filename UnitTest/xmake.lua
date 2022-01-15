target("CppUtils-UnitTest")
	set_kind("$(kind)")
	set_policy("build.merge_archive", true)
	add_deps(
		"CppUtils-Log",
		"CppUtils-Parameters"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
