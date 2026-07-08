project "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir (outputdir)
	objdir (outputintdir)

	files
	{
		"src/**.h",
		"src/**.hpp",
		"src/**.cpp"
	}

	includedirs
	{
		"src",
		"%{wks.location}/Eis/src"
	}

	externalwarnings "off"
	externalincludedirs
	{
		"%{wks.location}/Eis/vendor/glm",
		"%{wks.location}/Eis/vendor/imgui",
		"%{wks.location}/Eis/vendor/imguizmo",
		"%{wks.location}/Eis/vendor/spdlog/include",
		"%{wks.location}/Eis/vendor/entt/include"
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

	filter "action:vs*"
		files
		{
			"%{wks.location}/Eis/vendor/glm/util/glm.natvis",
            "%{wks.location}/Eis/vendor/imgui/misc/debuggers/imgui.natvis",
            "%{wks.location}/Eis/vendor/imgui/misc/debuggers/imgui.natstepfilter",
		}
	filter "toolset:msc*"
		multiprocessorcompile "on"
		buildoptions { "/utf-8", "/Zc:preprocessor" }
		linkoptions { "/IGNORE:4099" }

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