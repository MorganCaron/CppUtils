target("CppUtils-Container")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Type",
		"CppUtils-Hash"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
