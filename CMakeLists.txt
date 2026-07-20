-cmake_minimum_required(VERSION 3.22)

project(PluginMinecraft)
set(SHADOWHOOK_MODE "shared")
add_subdirectory(
    shadowhook/src/main/cpp
    shadowhook-build
)

file(GLOB imguis "imgui/*.cpp")

add_library(PluginMinecraft SHARED
    PluginMinecraft/main.cpp
    ${imguis}
)

target_include_directories(PluginMinecraft PRIVATE
    imgui
    shadowhook/src/main/cpp/include
)

target_link_libraries(PluginMinecraft
    log
    EGL
    GLESv2
    shadowhook
)

target_compile_features(PluginMinecraft PRIVATE cxx_std_20)