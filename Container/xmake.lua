target("CppUtils-Container")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Type",
		"CppUtils-String"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
