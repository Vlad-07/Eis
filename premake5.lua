outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

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
		include "Eis/vendor/spdlog"
		include "Eis/vendor/premake"
	group ""


include "Eis"


project "Editor"
	location "%{prj.name}" 
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"Eis/src"
	}

	externalincludedirs
	{
		"Eis/vendor/glm",
		"Eis/vendor/imgui",
		"Eis/vendor/imguizmo",
		"Eis/vendor/spdlog/include",
		"Eis/vendor/entt/include"
	}

	links
	{
		"Eis"
	}

	defines
	{
		"IMGUI_USER_CONFIG=\"Eis/ImGui/ImGuiConfig.h\"",
		"GLM_ENABLE_EXPERIMENTAL"
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
	filter ""