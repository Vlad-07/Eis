project "Eis"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir (outputdir)
	objdir (outputintdir)

	pchheader "Eispch.h"
	pchsource "src/Eispch.cpp"

	files
	{
		"src/**.h",
		"src/**.hpp",
		"src/**.cpp",

		"vendor/stb_image/stb_image.h",
		"vendor/stb_image/stb_image.cpp"
	}

	includedirs
	{
		"src"
	}

	externalwarnings "off"
	externalincludedirs
	{
		"vendor/GLFW/include",
		"vendor/Glad/include",
		"vendor/imgui",
		"vendor/imguizmo",
		"vendor/glm",
		"vendor/spdlog/include",
		"vendor/entt/include",
		"vendor/json/include",
		"vendor/stb_image",
		"vendor/SPIRV-Cross",
		"vendor/shaderc/include",
		"vendor/rapidhash/include",
		"vendor/fastgltf/include"
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
		"spdlog",
		"fastgltf",
		"SPIRV-Cross",
		"opengl32.lib"
	}

	filter "toolset:msc*"
		multiprocessorcompile "on"
		buildoptions { "/utf-8", "/Zc:preprocessor" }

	filter "system:windows"
		systemversion "latest"

	filter { "system:windows", "configurations:Debug" }
		links { "vendor/shaderc/lib/shaderc_combinedd.lib" }
	filter { "system:windows", "configurations:Release" }
		links { "vendor/shaderc/lib/shaderc_combined.lib" }

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