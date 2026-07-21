cmake_minimum_required(VERSION 3.22)

project(PluginMinecraft)

file(GLOB imguis "imgui/*.cpp")
file(GLOB mains "PluginMinecraft/*.cpp")
file(GLOB guis "PluginMinecraft/Gui/*.cpp")

add_library(PluginMinecraft SHARED
    ${mains}
    ${imguis}
    ${guis}
)

target_include_directories(PluginMinecraft PRIVATE
    imgui
)

target_link_libraries(PluginMinecraft
    log
    android
    EGL
    GLESv2
    GLESv3
    dl
)

target_compile_features(PluginMinecraft PRIVATE cxx_std_20)