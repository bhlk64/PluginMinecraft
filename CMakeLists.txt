cmake_minimum_required(VERSION 3.22)

project(PluginMinecraft)

file(GLOB imguis "imgui/*.cpp")
file(GLOB mains "PluginMinecraft/*.cpp")

add_library(PluginMinecraft SHARED
    ${mains}
    ${imguis}
)

target_include_directories(PluginMinecraft PRIVATE
    imgui
)

target_link_libraries(PluginMinecraft
    log
    EGL
    GLESv2
)

target_compile_features(PluginMinecraft PRIVATE cxx_std_20)