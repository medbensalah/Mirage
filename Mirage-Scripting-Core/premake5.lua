project "Mirage-Scripting-Core"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.8"

    targetdir ("%{wks.location}/Mirage Editor/Resources/Scripts")
    objdir ("%{wks.location}/Mirage Editor/Resources/Scripts/intermediates")

    files
    {
        "Source/**.cs",
        "Properties/**.cs",
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