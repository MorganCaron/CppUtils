target("CppUtils-UnitTests", function()
	set_kind("binary")
	add_deps("CppUtils")
	add_files("**.cpp")
	add_files("**.mpp", { public = true })

	if is_plat("linux") then
		add_packages("wayland", "wayland-protocols")
		add_rules("wayland.protocols")
	end
end)
