project "SPIRV-Cross"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir (outputdir)
	objdir (outputintdir)

	files
	{
		"GLSL.std.450.h",
		"spirv_common.hpp",
		"spirv_cross_containers.hpp",
		"spirv_cross_error_handling.hpp",
		"spirv.hpp",
		"spirv_cross.hpp",
		"spirv_cross.cpp",
		"spirv_parser.hpp",
		"spirv_parser.cpp",
		"spirv_cross_parsed_ir.hpp",
		"spirv_cross_parsed_ir.cpp",
		"spirv_cfg.hpp",
		"spirv_cfg.cpp",

		"spirv_cpp.cpp",
		"spirv_cpp.hpp",

		"spirv_reflect.cpp",
		"spirv_reflect.hpp",

		"spirv_glsl.hpp",
		"spirv_glsl.cpp"
	}

	filter "toolset:msc*"
		multiprocessorcompile "on"
		buildoptions { "/utf-8" }

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