project "engine"
    kind "ConsoleApp"
    language "C++"

    targetdir("../bin/" .. outputdir .. "/%{prj.name}")
    objdir("../bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "source/**.cpp",
        "source/**.h"
    }

    includedirs {
        "include"
    }

    libdirs { "libs" }
    
    links { 
        "OpenGL32",
        "glew32",
        "glew32s",
        "glfw3",
        "soil2"
    }

    defines { "WINDOWS" }

    filter { "configurations:Debug" }
        runtime "Debug"
        symbols "on"

    filter { "configurations:Release" }
        runtime "Release"
        optimize "on"

    filter { "system:windows" }
        postbuildcommands { "{COPYFILE} dll/glew32.dll ../bin/" .. outputdir .. "/%{prj.name}" }
        postbuildcommands { "{COPYDIR} assets ../bin/" .. outputdir .. "/%{prj.name}/assets"}