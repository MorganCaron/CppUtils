target("CppUtils-Hash")
	set_kind("headeronly")
	add_deps(
		"CppUtils-String"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
