project "runtime"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    files
    {
        "src/**.cpp",
        "src/**.hpp"
    }

    includedirs
    {
        "%{wks.location}/kon/src/",
        "%{wks.location}/external/glfw/include/",
        "%{wks.location}/external/glm",
        "%{IncludeDir.VulkanSDK}"
    }

    libdirs { "%{LibraryDir.VulkanSDK}" }

	links
	{
        "kon",
        "glfw",
        "%{Library.Vulkan}"
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
