target("CppUtils-Language-ASM-Compiler")
	set_kind("static")
	set_policy("build.merge_archive", true)
	add_deps(
		"CppUtils-Log",
		"CppUtils-Language-Parser",
		"CppUtils-Language-Lexer",
		"CppUtils-Language-Compiler",
		"CppUtils-Language-ASM-Lexer"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
