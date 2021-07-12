if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
	message(FATAL_ERROR "Prevented in-tree build.")
endif()

cmake_minimum_required(VERSION 3.1)
project(nocraft)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#
# Modules
#

set(NC_SRC_MODULES
    src/main.cpp
    src/chunk.h
    src/chunk.cpp
    src/rendering.h
    src/rendering.cpp
    src/shader.h
    src/shader.cpp
    src/camera.h
    src/world.h
    src/world.cpp
    src/random.h
    src/random.cpp
    src/vec.h
    src/xmath.h
    src/math_simd.h
)

include_directories(
    src
    external
    external/glad/include
)

add_executable(nocraft ${NC_SRC_MODULES})

file(COPY ${CMAKE_SRC_DIR}assets DESTINATION ${CMAKE_BINARY_DIR})

if (MSVC)
    target_compile_options(nocraft PRIVATE /W3)
else()
    target_compile_options(nocraft PRIVATE -Wall -Wextra -fno-exceptions -fno-rtti)
endif()

#
# External libraries
#

#
# opengl
#

find_package(OpenGL REQUIRED)

#
# GLFW
#

add_subdirectory(external/glfw)

#
# GLAD
#

set(NC_SRC_GLAD
    external/glad/src/glad.c
    external/glad/include/glad/glad.h
)

add_library(glad ${NC_SRC_GLAD})


target_link_libraries(nocraft
    opengl32
    glad
    glfw
)
