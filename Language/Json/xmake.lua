target("CppUtils-Language-Json")
	set_kind("$(kind)")
	set_policy("build.merge_archive", true)
	add_deps(
		"CppUtils-Language-Lexer"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
