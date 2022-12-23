workspace "Eis"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Distrib"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Eis/vendor/GLFW/include"
IncludeDir["Glad"] = "Eis/vendor/Glad/include"
IncludeDir["ImGui"] = "Eis/vendor/imgui"
IncludeDir["glm"] = "Eis/vendor/glm"
IncludeDir["stb_image"] = "Eis/vendor/stb_image"

group "Dependencies"
	include "Eis/vendor/GLFW"
	include "Eis/vendor/Glad"
	include "Eis/vendor/imgui"

group ""

project "Eis"
	location "Eis"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "Eispch.h"
	pchsource "Eis/src/Eispch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"

	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
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

		defines
		{
			"EIS_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
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

project "Sandbox"
	location "Sandbox" 
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.hpp"
	}

	includedirs
	{
		"Eis/src",
		"Eis/vendor/spdlog/include",
		"Eis/vendor/imgui",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Eis"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"EIS_PLATFORM_WINDOWS"
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