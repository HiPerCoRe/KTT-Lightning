-- Helper functions to find compute API headers and libraries
function linkCuda()
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
    
    links {"cuda"}
    return true
end

function linkRttr()
    includedirs {"Libraries/rttr-0.9.6/include"}
    
    if os.target() == "linux" then
        libdirs {"Libraries/rttr-0.9.6/lib/linux"}
    else
        libdirs {"Libraries/rttr-0.9.6/lib/windows"}
    end
    
    filter "configurations:Debug"
        links {"rttr_core_d"}
    
    filter "configurations:Release"
        links {"rttr_core"}
    
    filter {}
end

function linkKtt()
    includedirs {"Libraries/KTT-2.0.1/Include"}
    
    if os.target() == "linux" then
        filter "configurations:Debug"
            libdirs {"Libraries/KTT-2.0.1/Lib/Linux/Debug"}
    
        filter "configurations:Release"
            libdirs {"Libraries/KTT-2.0.1/Lib/Linux/Release"}
            
        filter {}
    else
        filter "configurations:Debug"
            libdirs {"Libraries/KTT-2.0.1/Lib/Windows/Debug"}
    
        filter "configurations:Release"
            libdirs {"Libraries/KTT-2.0.1/Lib/Windows/Release"}
            
        filter {}
    end
    
    links {"ktt"}
end

function linkLibraries()
    local cudaFound = linkCuda()
    
    if not cudaFound then
        error("CUDA libraries were not found. Please ensure that CUDA_PATH is correctly set in the environment variables")
    end
    
    linkKtt()
    linkRttr()
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
    files {"Source/**", "Libraries/KTT-2.0.1/**", "Libraries/rttr-0.9.6/**"}
    includedirs {"Source"}
    defines {"KTTL_LIBRARY", "KTTL_SOURCE=\"%{_WORKING_DIR}/Source/\""}
    targetname("ktt-lightning")
    linkLibraries()

-- Examples
project "Playground"
    kind "ConsoleApp"
    files {"Examples/Playground/Playground.cpp"}
    includedirs {"Source"}
    links {"ktt-lightning"}
    linkCuda()
