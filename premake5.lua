include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Mirage"
	architecture "x86_64"
    startproject "Mirage Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	solution_items
    {
        ".editorconfig"
    }
	
	flags
	{
		"MultiProcessorCompile"
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "vendor/premake"
    include "Mirage/vendor/Box2D"
    include "Mirage/vendor/GLFW"
    include "Mirage/vendor/Glad"
    include "Mirage/vendor/ImGui"
    include "Mirage/vendor/yaml-cpp"
group ""

group "Core"
	include "Mirage"
	include "Mirage-Scripting-Core"
group ""


group "Tools"
	include "Mirage Editor"
group ""


group "Misc"
	include "Sandbox"
group ""
