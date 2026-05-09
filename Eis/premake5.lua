project "Eis"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "Eispch.h"
	pchsource "src/Eispch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",

		"vendor/entt/include/entt.hpp",

		"vendor/stb_image/stb_image.h",
		"vendor/stb_image/stb_image.cpp",
	}

	includedirs
	{
		"src"
	}

	externalincludedirs
	{
		"vendor/GLFW/include",
		"vendor/Glad/include",
		"vendor/imgui",
		"vendor/glm",
		"vendor/spdlog/include",
		"vendor/entt/include",
		"vendor/stb_image"
	}

	defines
	{
		"IMGUI_USER_CONFIG=\"Eis/ImGui/ImGuiConfig.h\"",
		"GLFW_INCLUDE_NONE",
		"GLM_ENABLE_EXPERIMENTAL"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "toolset:msc*"
		buildoptions { "/utf-8" }
		multiprocessorcompile "on"

	filter "system:windows"
		systemversion "latest"
		links { "Ws2_32.lib" }

	filter "configurations:Debug"
		defines "EIS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EIS_RELEASE"
		runtime "Release"
		optimize "on"
		linktimeoptimization "on"