target("CppUtils-Parameters")
	set_kind("static")
	set_policy("build.merge_archive", true)
	add_deps(
		"CppUtils-Language-Parser",
		"CppUtils-Language-Lexer"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
