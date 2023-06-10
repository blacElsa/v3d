cmake_minimum_required(VERSION 3.26)

project(v3d
        VERSION "0.0.1"
        DESCRIPTION "simple 3d viewer"
        LANGUAGES CXX C)

include(cmake/PreventInSourceBuild.cmake)
include(cmake/ProjectOptions.cmake)

v3d_setup_options()
v3d_global_options()

# Add dependencies

v3d_local_options()

# target_compile_features(v3d_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

add_library(v3d::v3d_options ALIAS v3d_options)
add_library(v3d::v3d_warnings ALIAS v3d_warnings)

add_subdirectory(src)