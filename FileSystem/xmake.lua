target("CppUtils-FileSystem")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Type",
		"CppUtils-Thread"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
