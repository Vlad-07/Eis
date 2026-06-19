project "ImGui"
	kind "StaticLib"
	language "C++"
    cppdialect "C++20"
    staticruntime "off"

	targetdir (outputdir)
	objdir (outputintdir)

	files
	{
		"imgui.h",
		"imgui.cpp",
		"imgui_draw.cpp",
		"imgui_internal.h",
		"imgui_tables.cpp",
		"imgui_widgets.cpp",
		"imstb_rectpack.h",
		"imstb_textedit.h",
		"imstb_truetype.h",
		"imgui_demo.cpp",

        "misc/cpp/imgui_stdlib.cpp",

        "../imguizmo/ImGuizmo.h",
		"../imguizmo/ImGuizmo.cpp"
	}

	includedirs
	{
		"./",
		"../GLFW/include",
		"../glm",
		"../../src/Eis/ImGui"
	}

	defines
	{
		"IMGUI_USER_CONFIG=\"ImGuiConfig.h\""
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
