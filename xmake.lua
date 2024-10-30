set_project("CppUtils")
set_version("0.0.0", {build = "%Y%m%d%H%M"})
set_license("LGPL3")
set_languages("clatest", "cxxlatest")
set_warnings("allextra", "pedantic", "error")
add_cxflags("clang::-Wconversion", "clang::-Wfatal-errors", "clang::-Wno-deprecated-declarations -Wno-unknown-attributes")
add_cxflags("clang::-fcolor-diagnostics", "clang::-fansi-escape-codes", "gcc::-fdiagnostics-color=always")
-- add_cxflags("-Wno-gnu-statement-expression-from-macro-expansion -Wno-gnu-statement-expression", {tools = { "clang", "gcc" })
set_optimize("fastest")

add_cxxflags("clang::-fexperimental-library", {force = true}) -- Pour avoir std::jthread
add_cxxflags("cl::/EHsc", {force = true}) -- Pour avoir std::jthread

if is_plat("windows") and not is_plat("mingw") then
	set_runtimes(is_mode("debug") and "MDd" or "MD")
	add_defines("NOMINMAX", "VC_EXTRALEAN", "WIN32_LEAN_AND_MEAN", "_CRT_SECURE_NO_WARNINGS", { public = true })
	add_cxflags("cl::/wd4251", {force = true}) -- ‘identifier’ : class ‘type’ needs to have dll-interface to be used by clients of class ‘type2’
	add_syslinks("Ws2_32")
elseif is_plat("linux", "macosx") then
	add_syslinks("pthread", "dl")
end

add_rules(
	"mode.debug",
	"mode.release",
	"mode.releasedbg",
	"mode.minsizerel",
	"mode.check",
	"mode.profile",
	"mode.coverage",
	"mode.valgrind")

if is_mode("debug") then
	set_policy("build.sanitizer.address", true)
	set_policy("build.sanitizer.thread", true)
	set_policy("build.sanitizer.undefined", true)
end

add_requires("wayland")
add_requires("wayland-protocols")
includes("xmake/rules/wayland-protocols.lua")

option("enable_tests")
option("enable_moduleonly", {default = true})

target("CppUtils", function()
	if get_config("enable_moduleonly") then
		set_kind("moduleonly")
	else
	  set_kind("$(kind)")
	end
	
	add_packages("wayland", "wayland-protocols", { public = true })
	-- Ajouter "wayland-cursor"

	if is_plat("linux") then
		add_rules("wayland.protocols")
		on_load(function(target)
			local wayland_protocols_package = target:pkg("wayland-protocols")
			if not wayland_protocols_package then
				os.raise("wayland-protocols package not found")
			end
			local wayland_protocols_dir = path.join(wayland_protocols_package:installdir() or "/usr", "share", "wayland-protocols")
			assert(os.isdir(wayland_protocols_dir), "wayland protocols directory not found")

			local protocols = {
				path.join(wayland_protocols_dir, "stable", "xdg-shell", "xdg-shell.xml"),
				path.join(wayland_protocols_dir, "unstable", "xdg-decoration", "xdg-decoration-unstable-v1.xml"),
				path.join(wayland_protocols_dir, "unstable", "pointer-constraints", "pointer-constraints-unstable-v1.xml"),
				path.join(wayland_protocols_dir, "unstable", "relative-pointer", "relative-pointer-unstable-v1.xml"),
			}

			for _, protocol in ipairs(protocols) do
				if os.exists(protocol) then
					target:add("files", protocol)
				else
					print("Protocol file not found:", protocol)
				end
			end
		end)
	end

	add_files("modules/**.mpp", { public = true })
	add_includedirs("include", { public = true })
	add_headerfiles("include/(CppUtils/**.hpp)")
	add_headerfiles("include/(Stl/**.hpp)")
end)

if has_config("enable_tests") then
	includes("tests")
end
