outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "vendor/GLFW/include"
IncludeDir["Glad"] = "vendor/Glad/include"
IncludeDir["ImGui"] = "vendor/imgui"
IncludeDir["ImPlot"] = "vendor/implot"
IncludeDir["glm"] = "vendor/glm"
IncludeDir["spdlog"] = "vendor/spdlog/include"
IncludeDir["stb_image"] = "vendor/stb_image"
IncludeDir["stb_image_resize"] = "vendor/stb_image_resize"
IncludeDir["stb_image_write"] = "vendor/stb_image_write"
IncludeDir["GameNetworkingSockets"] = "vendor/GameNetworkingSockets/include/steam"

project "Eis"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "Eispch.h"
	pchsource "src/Eispch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/stb_image_resize/**.h",
		"vendor/stb_image_resize/**.cpp",
		"vendor/stb_image_write/**.h",
		"vendor/stb_image_write/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl"

	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImPlot}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.stb_image_resize}",
		"%{IncludeDir.stb_image_write}",
		"%{IncludeDir.GameNetworkingSockets}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}
	
	filter "system:windows"
		systemversion "latest"
		defines "EIS_PLATFORM_WINDOWS"
    	links { "Ws2_32.lib" }

	filter { "system:windows", "configurations:Debug" }	
		links
		{
			"vendor/GameNetworkingSockets/.bin/Windows/Debug/GameNetworkingSockets.lib"
		}

	filter { "system:windows", "configurations:Release or configurations:Dist" }	
		links
		{
			"vendor/GameNetworkingSockets/.bin/Windows/Release/GameNetworkingSockets.lib"
		}

	filter "configurations:Debug"
		defines "EIS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EIS_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distrib" 
		defines "EIS_DISTRIB"
		runtime "Release"
		optimize "on"