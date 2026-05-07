cmake_minimum_required(VERSION 3.20)


# SETUP

project(
    simple_inrunner 
    VERSION 1.0
    DESCRIPTION "FEM Solver for simple inrunner PMSM"
    LANGUAGES CXX
    )


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)


add_library(eigen INTERFACE)
target_include_directories(eigen INTERFACE
    ${CMAKE_SOURCE_DIR}/deps/eigen-5.0.0
)


add_library(gmsh SHARED IMPORTED)
target_include_directories(gmsh INTERFACE
    ${CMAKE_SOURCE_DIR}/deps/gmsh-4.15.2-Linux64-sdk/include
)


set_target_properties(gmsh PROPERTIES
    IMPORTED_LOCATION
    ${CMAKE_SOURCE_DIR}/deps/gmsh-4.15.2-Linux64-sdk/lib/libgmsh.so
)


# Executable

add_executable(
    simple_inrunner
    ${SRC_DIR}/main.cpp
)


target_link_libraries(
    simple_inrunner 
    PRIVATE eigen 
    PRIVATE gmsh
)

