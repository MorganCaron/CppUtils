target("CppUtils-Terminal")
	set_kind("$(kind)")
	add_options("libc++_static", "libc++_shared")
	add_deps(
		"CppUtils-Platform",
		"CppUtils-Container"
	)
	set_policy("build.merge_archive", true)
	add_defines("DLL_EXPORT")
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
	add_files("src/*.cpp")
