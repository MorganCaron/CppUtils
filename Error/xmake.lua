target("CppUtils-Error")
	set_kind("headeronly")
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")