target("CppUtils-Language-IR-Compiler")
	set_kind("$(kind)")
	set_policy("build.merge_archive", true)
	add_deps(
		"CppUtils-Log",
		"CppUtils-Language-Parser",
		"CppUtils-Language-Lexer",
		"CppUtils-Language-Compiler",
		"CppUtils-Language-IR-Lexer"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
