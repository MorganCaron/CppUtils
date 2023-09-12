includes(
	"chrono",
	"expected",
	"format",
	"functional",
	"print")

target("CppUtils-Stl")
	set_kind("headeronly")
	add_deps(
		"CppUtils-Stl-chrono",
		"CppUtils-Stl-expected",
		"CppUtils-Stl-format",
		"CppUtils-Stl-functional",
		"CppUtils-Stl-print"
	)
	add_includedirs("include", { public = true })
	add_headerfiles("include/**.hpp")
