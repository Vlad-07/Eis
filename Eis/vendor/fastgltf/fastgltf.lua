project "fastgltf"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir (outputdir)
	objdir (outputintdir)

	files
	{
		"include/fastgltf/base64.hpp",
        "include/fastgltf/core.hpp",
        "include/fastgltf/dxmath_element_traits.hpp",
        "include/fastgltf/glm_element_traits.hpp",
        "include/fastgltf/tools.hpp",
        "include/fastgltf/types.hpp",
        "include/fastgltf/util.hpp",
        "include/fastgltf/math.hpp",

		"src/fastgltf.cpp",
        "src/base64.cpp",
        "src/io.cpp",

		"simdjson/simdjson.h", -- TODO: this is stupid
		"simdjson/simdjson.cpp"
	}

	includedirs
	{
		"include",
		"simdjson"
	}

	filter "toolset:msc*"
		multiprocessorcompile "on"

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
		linktimeoptimization "on"
	filter ""