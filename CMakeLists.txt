
# Specify CMAKE version.
cmake_minimum_required(VERSION 3.10)

# Specify the C++ standard.
set(CMAKE_CXX_STANDARD 11)

# Project name.
project(renderer)

# Add headers.
include_directories("${CMAKE_SOURCE_DIR}/src")
include_directories("${CMAKE_SOURCE_DIR}/src/thirdparty/glad")
include_directories("${CMAKE_SOURCE_DIR}/src/thirdparty/stbimage")
include_directories("${CMAKE_SOURCE_DIR}/src/thirdparty/glm")

# Libraries.
add_library(GLAD "${CMAKE_SOURCE_DIR}/src/thirdparty/glad/glad.c")
add_library(STB_IMAGE "${CMAKE_SOURCE_DIR}/src/thirdparty/stbimage/stb_image.cpp")

find_library(assimp REQURED)
include_directories( ${ASSIMP_INCLUDE_DIRS}) # TODO: remove?

# Add sources.
set(SOURCES src/renderer.cpp)

# Executable.
add_executable(renderer ${SOURCES})

# TODO: add linking above?
target_link_libraries(renderer assimp STB_IMAGE GLAD -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl)
