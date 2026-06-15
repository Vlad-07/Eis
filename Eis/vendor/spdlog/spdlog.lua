project "spdlog"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir (outputdir)
	objdir (outputintdir)

	files
	{
		--should include all headers?
		"include/spdlog/spdlog.h",

		"src/async.cpp",
		"src/bundled_fmtlib_format.cpp",
		"src/cfg.cpp",
		"src/color_sinks.cpp",
		"src/file_sinks.cpp",
		"src/spdlog.cpp",
		"src/stdout_sinks.cpp"
	}

	includedirs
	{
		"include"
	}

	defines
	{
		"SPDLOG_COMPILED_LIB"
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