cmake_minimum_required(VERSION 3.22)

project(PluginMinecraft)

add_library(PluginMinecraft SHARED
    PluginMinecraft/hook.cpp
)

target_link_libraries(PluginMinecraft
    log
)

target_compile_features(PluginMinecraft PRIVATE cxx_std_20)