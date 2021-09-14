-- Helper functions to find compute API headers and libraries
function findLibrariesNvidia()
    local path = os.getenv("CUDA_PATH")
    
    if not path then
        return false
    end
    
    includedirs {"$(CUDA_PATH)/include"}
        
    if os.target() == "linux" then
        libdirs {"$(CUDA_PATH)/lib64"}
    else
        libdirs {"$(CUDA_PATH)/lib/x64"}
    end
    
    links {"cuda", "nvrtc"}
    return true
end

function linkLibraries()
    local librariesFound = findLibrariesNvidia()
    
    if not librariesFound then
        error("Compute API libraries were not found. Please ensure that path to the SDK is correctly set in the environment variables:\nCUDA_PATH for Nvidia")
    end
    
    includedirs {"Libraries/KTT-2.0.1/Include"}
    
    if os.target() == "linux" then
        libdirs {"Libraries/KTT-2.0.1/Lib/Linux"}
    else
        libdirs {"Libraries/KTT-2.0.1/Lib/Windows"}
    end
    
    links {"ktt"}
end

-- Project configuration
workspace "Ktt-Lightning"
    configurations {"Release", "Debug"}
    platforms {"x86_64"}
    architecture "x86_64"
    
    location "Build"
    language "C++"
    cppdialect "C++17"
    warnings "Extra"
    
    filter "configurations:Debug"
        defines {"KTTL_CONFIGURATION_DEBUG"}
        optimize "Off"
        symbols "On"
    
    filter "configurations:Release"
        defines {"KTTL_CONFIGURATION_RELEASE"}
        optimize "Full"
        symbols "Off"
    
    filter "action:vs*"
        buildoptions {"/Zc:__cplusplus"}
        
    filter {}
    
    targetdir("Build/%{cfg.platform}_%{cfg.buildcfg}")
    objdir("Build//%{cfg.platform}_%{cfg.buildcfg}/obj")
    
-- Library configuration
project "Ktt-Lightning"
    kind "SharedLib"
    files {"Source/**", "Libraries/KTT-2.0.1/**"}
    includedirs {"Source"}
    defines {"KTTL_LIBRARY"}
    targetname("ktt-lightning")
    linkLibraries()

-- Examples
project "Playground"
    kind "ConsoleApp"
    files {"Examples/Playground/Playground.cpp"}
    includedirs {"Source"}
    links {"ktt-lightning"}
    