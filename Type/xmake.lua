target("CppUtils-Type")
	set_kind("headeronly")
	add_deps(
		"CppUtils-External",
		"CppUtils-Hash"
	)
	set_policy("build.merge_archive", true)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
