include "./vendor/premake/premake_customization/solution_items.lua"

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

--include directories relative to solution directory
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Mirage/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Mirage/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Mirage/vendor/ImGui"
IncludeDir["glm"] = "%{wks.location}/Mirage/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Mirage/vendor/stb_image"
IncludeDir["enTT"] = "%{wks.location}/Mirage/vendor/enTT/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Mirage/vendor/yaml-cpp/include"

group "Dependencies"
    include "vendor/premake"
    include "Mirage/vendor/GLFW"
    include "Mirage/vendor/Glad"
    include "Mirage/vendor/ImGui"
    include "Mirage/vendor/yaml-cpp"
group ""

include "Mirage"
include "Sandbox"
include "Mirage Editor"
