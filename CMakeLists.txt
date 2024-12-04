cmake_minimum_required(VERSION 3.0.0)
project(AlgoVisual VERSION 0.1.0)

# Compiler options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
link_directories(libs/glfw)

# Include directories
include_directories(
    libs/glad
    libs/KHR
    libs/imgui
    libs/glfw
    include
)

# Source files
file(GLOB_RECURSE SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/algorithms/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/visualization/*.cpp
)

# Main executable
add_executable(${PROJECT_NAME} ${SOURCES})

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    ${OPENGL_gl_LIBRARY}
    glfw3
)

# Resources
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})

# Testing
enable_testing()
add_subdirectory(tests)
