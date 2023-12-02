target("CppUtils-UnitTest")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Log",
		"CppUtils-String",
		"CppUtils-Parameters"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
