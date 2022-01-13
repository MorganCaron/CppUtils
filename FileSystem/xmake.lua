target("CppUtils-FileSystem")
	set_kind("static")
	set_policy("build.merge_archive", true)
	add_deps("CppUtils-Thread")
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
