target("CppUtils-Hash")
	set_kind("static")
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
