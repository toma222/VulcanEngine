
include "deps.lua"

workspace "Kon"
    architecture "x86_64"
	startproject "kon"
    configurations { "Debug", "Release" }

    

    filter "configurations:Debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "external/glfw"
group ""

group "Core"
	include "kon"
group ""

group "Tools"
    include "testing"
    include "runtime"
group ""
