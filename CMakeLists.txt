cmake_minimum_required(VERSION 3.20)


# SETUP
project(
    simple_outrunner
    VERSION 1.0
    DESCRIPTION "FEM Solver for simple outrunner PMSM"
    LANGUAGES CXX
    )

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps)

add_library(toml INTERFACE)
target_include_directories(toml INTERFACE
    ${DEPS_DIR}/toml
)

add_library(gmsh SHARED IMPORTED)
target_include_directories(gmsh INTERFACE
    ${DEPS_DIR}/gmsh/include
)


if(APPLE)
    set(GMSH_LIB ${CMAKE_SOURCE_DIR}/deps/gmsh/lib/libgmsh.dylib)

elseif(UNIX)
    set(GMSH_LIB ${CMAKE_SOURCE_DIR}/deps/gmsh/lib/libgmsh.so)

endif()

set_target_properties(gmsh PROPERTIES
    IMPORTED_LOCATION ${GMSH_LIB}
)

# Executable
add_executable(
    simple_outrunner
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/preprocessor/preprocessor.cpp
    ${SRC_DIR}/preprocessor/utils/utils.cpp
    ${SRC_DIR}/preprocessor/params/config.cpp
    ${SRC_DIR}/preprocessor/params/stator.cpp
    ${SRC_DIR}/preprocessor/params/rotor.cpp
    ${SRC_DIR}/preprocessor/builders/origin.cpp
    ${SRC_DIR}/preprocessor/builders/stator.cpp
    ${SRC_DIR}/preprocessor/builders/airgap.cpp
    ${SRC_DIR}/preprocessor/builders/rotor.cpp
    ${SRC_DIR}/preprocessor/processors/meshing.cpp
    ${SRC_DIR}/preprocessor/processors/naming.cpp
)

target_include_directories(
    simple_outrunner
    PRIVATE ${INCLUDE_DIR}
)

target_link_libraries(
    simple_outrunner
    PRIVATE gmsh
    PRIVATE toml
)

