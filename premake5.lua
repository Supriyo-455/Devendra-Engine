workspace "DevendraEngine"
    architecture "x64"

    configurations {
        "Debug",
        "Release"
    }

    startproject "engine"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "engine"

newaction {
    trigger = "clean",
    description = "Remove all binaries and intermediate binaries, and vs files.",
    execute = function()
        print("Removing binaries")
        os.rmdir("./bin")
        print("Removing intermediate binaries")
        os.rmdir("./bin-int")
        print("Removing project files")
        os.rmdir("./.vs")
        os.remove("**.sln")
        os.remove("**.vcxproj")
        os.remove("**.vcxproj.filters")
        os.remove("**.vcxproj.user")
        print("Done")
    end
}