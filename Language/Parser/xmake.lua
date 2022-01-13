target("CppUtils-Language-Parser")
	set_kind("static")
	set_policy("build.merge_archive", true)
	add_deps(
		"CppUtils-Type",
		"CppUtils-Graph",
		"CppUtils-String"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
