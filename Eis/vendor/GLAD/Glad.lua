project "Glad"
	kind "StaticLib"
	language "C"
	staticruntime "off"

	targetdir (outputdir)
	objdir (outputintdir)

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	includedirs
	{
		"include"
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