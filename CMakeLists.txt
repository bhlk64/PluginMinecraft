cmake_minimum_required(VERSION 3.22)

project(PluginMinecraft)

add_subdirectory(Dobby)
file(GLOB imguis "imgui/*.cpp")

add_library(PluginMinecraft SHARED
    PluginMinecraft/main.cpp
    ${imguis}
)

target_include_directories(PluginMinecraft PRIVATE
    imgui
    Dobby/include
)

target_link_libraries(PluginMinecraft
    log
    EGL
    GLESv2
    dobby
)

target_compile_features(PluginMinecraft PRIVATE cxx_std_20)