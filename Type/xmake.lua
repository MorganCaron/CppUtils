target("CppUtils-Type")
	set_kind("static")
	set_policy("build.merge_archive", true)
	add_deps(
		"CppUtils-External",
		"CppUtils-Hash"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
	add_files("src/*.cpp")
