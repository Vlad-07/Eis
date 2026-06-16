project "Premake"
	kind "Utility"

	targetdir (outputdir)
	objdir (outputintdir)

	files
	{
		"%{wks.location}/**.lua",
	}

	postbuildmessage "Regenerating project files with Premake5!"
	postbuildcommands
	{
		"\"%{prj.location}bin/premake5\" %{_ACTION} --file=\"%{wks.location}Workspace.lua\""
	}

	filter "action:vs*"
		fastuptodate "off"
		excludefrombuild "on"
	filter ""