
project "testing"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    files
    {
        "src/**.cpp",
        "src/**.hpp"
    }

    links
    {
        "kon"
    }

    includedirs
    {
        "%{wks.location}/kon/src/"
    }

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    filter "configurations:Debug"
        defines "KN_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "KN_RELEASE"
        runtime "Release"
        optimize "on"