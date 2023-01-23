workspace("volta-chan")
    configurations({"Debug", "Release"})

project("volta-chan")
    kind("WindowedApp")
    language("C++")
    cppdialect("C++11")
    targetdir("bin/%{cfg.buildcfg}")

    libdirs({"/usr/local/lib"})
    links({"SDL2", "SDL2_image"})

    files({
        "**.cpp",
        "**.h"
    })

    removefiles({
        "lib/imgui/misc/freetype/imgui_freetype.*"
    })

    filter "configurations:Debug"
        defines({"DEBUG"})
        symbols("On")

    filter "configurations:Release"
        defines({"NDEBUG"})
        optimize("On")
