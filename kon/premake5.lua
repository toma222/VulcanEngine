project "kon"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    files
    {
        "src/**.cpp",
        "src/**.hpp"
    }

    includedirs
    {
        "src",
        "%{wks.location}/external/glfw/include",
        "%{wks.location}/external/glm",
        "%{wks.location}/external/stb",
		"%{wks.location}/external",
        "%{IncludeDir.VulkanSDK}"
    }

    libdirs { "%{LibraryDir.VulkanSDK}" }

	links
	{
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
