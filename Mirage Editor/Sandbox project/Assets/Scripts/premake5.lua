local MirageRootDir = '../../../..'
include (MirageRootDir .. "/vendor/premake/premake_customization/solution_items.lua")

workspace "Sandbox"
	architecture "x86_64"
    startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Sandbox"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.8"

    targetdir ("Binaries")
    objdir ("Intermediates")

    files
    {
        "Source/**.cs",
        "Properties/**.cs",
    }
    
    links
    {
    	"Mirage-Scripting-Core"
    }
	    
	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"
		
	filter "configurations:Release"
		optimize "On"
		symbols "Default"
		
	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"
		
group "Mirage"
	include (MirageRootDir .. "/Mirage-Scripting-Core")
group ""
