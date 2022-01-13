target("CppUtils-UnitTest")
	set_kind("static")
	set_policy("build.merge_archive", true)
	add_deps(
		"CppUtils-Log",
		"CppUtils-Parameters"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
