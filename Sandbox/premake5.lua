project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",        
    }

    includedirs
    {
        "%{wks.location}/Mirage/src",
        "%{wks.location}/Mirage/vendor/spdlog/include",
        "%{wks.location}/Mirage/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.enTT}"
    }

    links
    {
        "Mirage"
    }

    filter "system:windows"
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
