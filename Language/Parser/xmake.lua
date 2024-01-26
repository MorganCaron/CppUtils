target("CppUtils-Language-Parser")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Type",
		"CppUtils-Container",
		"CppUtils-String",
		"CppUtils-Terminal"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
