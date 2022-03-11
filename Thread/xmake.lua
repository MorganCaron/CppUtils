if is_plat("linux") then
	add_syslinks("pthread")
end

target("CppUtils-Thread")
	set_kind("headeronly")
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
