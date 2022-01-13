target("CppUtils-Chrono")
	set_kind("static")
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
