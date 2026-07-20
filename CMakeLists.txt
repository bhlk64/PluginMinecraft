cmake_minimum_required(VERSION 3.22)

project(PluginMinecraft)

add_library(PluginMinecraft SHARED
  PluginMinecraft/hook.cpp
)

target_compile_features(PluginMinecraft PRIVATE cxx_std_20)