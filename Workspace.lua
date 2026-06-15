outputsyntax = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
outputdir = "%{wks.location}/bin/" .. outputsyntax .. "/%{prj.name}"
outputintdir = "%{wks.location}/bin-int/" .. outputsyntax .. "/%{prj.name}"


workspace "Eis"
	startproject "Editor"
	architecture "x86_64"

	configurations
	{
		"Debug",
		"Release"
	}

	group "Dependencies"
		include "Eis/vendor/Glad/Glad.lua"
		include "Eis/vendor/GLFW/GLFW.lua"
		include "Eis/vendor/imgui/ImGui.lua"
		include "Eis/vendor/spdlog/spdlog.lua"
		include "Eis/vendor/premake/Premake.lua"
		include "Eis/vendor/SPIRV-Cross/SPIRV-Cross.lua"
	group ""


include "Eis/Eis.lua"
include "Editor/Editor.lua"