outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["Eis"] = "Eis/src"
IncludeDir["GLFW"] = "Eis/vendor/GLFW/include"
IncludeDir["Glad"] = "Eis/vendor/Glad/include"
IncludeDir["ImGui"] = "Eis/vendor/imgui"
IncludeDir["ImPlot"] = "Eis/vendor/implot"
IncludeDir["glm"] = "Eis/vendor/glm"
IncludeDir["spdlog"] = "Eis/vendor/spdlog/include"
IncludeDir["json"] = "Eis/vendor/json/include"
IncludeDir["entt"] = "Eis/vendor/entt/include"
IncludeDir["stb_image"] = "Eis/vendor/stb_image"
IncludeDir["GameNetworkingSockets"] = "Eis/vendor/GameNetworkingSockets/include"

Defines = {}
Defines["CRT"] = "_CRT_SECURE_NO_WARNINGS"
Defines["ImGuiCfg"] = "IMGUI_USER_CONFIG=\"Eis/ImGui/ImGuiConfig.h\""


workspace "Eis"
	startproject "Editor"
	architecture "x86_64"

	configurations
	{
		"Debug",
		"Release"
	}

	group "Dependencies"
		include "Eis/vendor/GLFW"
		include "Eis/vendor/Glad"
		include "Eis/vendor/imgui"
		include "Eis/vendor/implot"
	group ""


project "Eis"
	location "Eis"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("Eis/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Eis/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "Eispch.h"
	pchsource "Eis/src/Eispch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",

		"%{prj.name}/vendor/json/include/json.hpp",
		"%{prj.name}/vendor/json/include/json_fwd.hpp",

		"%{prj.name}/vendor/entt/include/entt.hpp",

		"%{prj.name}/vendor/stb_image/stb_image.h",
		"%{prj.name}/vendor/stb_image/stb_image.cpp",
	}

	defines
	{
		"%{Defines.CRT}",
		"%{Defines.ImGuiCfg}",
		"GLFW_INCLUDE_NONE",
		"GLM_ENABLE_EXPERIMENTAL"
	}

	includedirs
	{
		"%{IncludeDir.Eis}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImPlot}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.json}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.GameNetworkingSockets}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"ImPlot",
		"opengl32.lib"
	}

	filter "toolset:msc*"
		buildoptions { "/utf-8" }
		multiprocessorcompile "on"

	filter "system:windows"
		systemversion "latest"
		links { "Ws2_32.lib" }

	filter { "system:windows", "configurations:Debug" }	
		links
		{
			"Eis/vendor/GameNetworkingSockets/.bin/Windows/Debug/GameNetworkingSockets.lib"
		}

	filter { "system:windows", "configurations:Release" }	
		links
		{
			"Eis/vendor/GameNetworkingSockets/.bin/Windows/Release/GameNetworkingSockets.lib"
		}

	filter "configurations:Debug"
		defines "EIS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EIS_RELEASE"
		runtime "Release"
		optimize "on"
		linktimeoptimization "on"


project "Editor"
	location "Editor" 
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
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
		"%{prj.name}/src",
		"%{IncludeDir.Eis}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImPlot}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Eis"
	}

	defines
	{
		"%{Defines.CRT}",
		"%{Defines.ImGuiCfg}"
	}

	filter "toolset:msc*"
		buildoptions { "/utf-8" }
		multiprocessorcompile "on"

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "EIS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EIS_RELEASE"
		runtime "Release"
		optimize "on"
		linktimeoptimization "on"