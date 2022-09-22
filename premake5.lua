workspace "Mirage"
	architecture "x64"
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

--include directories relative to solution directory
IncludeDir = {}
IncludeDir["GLFW"] = "Mirage/vendor/GLFW/include"
IncludeDir["Glad"] = "Mirage/vendor/Glad/include"
IncludeDir["ImGui"] = "Mirage/vendor/ImGui"
IncludeDir["glm"] = "Mirage/vendor/glm"
IncludeDir["stb_image"] = "Mirage/vendor/stb_image"

include "Mirage/vendor/GLFW"
include "Mirage/vendor/Glad"
include "Mirage/vendor/ImGui"


project "Mirage"
    location "Mirage"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "MrgPch.h"
    pchsource "Mirage/src/MrgPch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",        
        "%{prj.name}/vendor/stb_image/**.cpp",        
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }
    
    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "Mirage/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}"
    }
    
    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "MRG_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "MRG_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "MRG_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "MRG_DIST"
        runtime "Release"
        optimize "on"
        
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
        
    }

    includedirs
    {
        "Mirage/src",
        "Mirage/vendor/spdlog/include",
        "Mirage/vendor",
        "%{IncludeDir.glm}"
    }

    links
    {
        "Mirage"
    }

    filter "system:windows"
        staticruntime "On"
        systemversion "latest"

    filter "configurations:Debug"
        defines "MRG_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "MRG_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "MRG_DIST"
        runtime "Release"
        optimize "on"
