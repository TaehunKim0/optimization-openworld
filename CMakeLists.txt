cmake_minimum_required(VERSION 3.20)
project(OpenWorldOptimization)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v6.0.2  
)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE) 
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT ON CACHE BOOL "" FORCE) 
FetchContent_MakeAvailable(assimp)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.5-docking
)
FetchContent_MakeAvailable(imgui)

set(IMGUI_SRC_DIR ${imgui_SOURCE_DIR})

add_library(imgui_lib STATIC
    ${IMGUI_SRC_DIR}/imgui.cpp
    ${IMGUI_SRC_DIR}/imgui_draw.cpp
    ${IMGUI_SRC_DIR}/imgui_tables.cpp
    ${IMGUI_SRC_DIR}/imgui_widgets.cpp
    ${IMGUI_SRC_DIR}/imgui_demo.cpp
    ${IMGUI_SRC_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_SRC_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_lib PUBLIC 
    ${IMGUI_SRC_DIR}
    ${IMGUI_SRC_DIR}/backends
    ${glfw_SOURCE_DIR}/include
)

target_link_libraries(imgui_lib PUBLIC 
    glfw 
    OpenGL::GL
)

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/**/*.cpp
)

add_executable(OptimizationDemo
    ${SOURCES}
    external/glad/src/glad.c
)

target_include_directories(OptimizationDemo PRIVATE
    src
    external/glad/include
    ${IMGUI_SRC_DIR}
    ${IMGUI_SRC_DIR}/backends
)

target_link_libraries(OptimizationDemo PRIVATE
    OpenGL::GL
    glfw
    glm::glm
    assimp::assimp
    imgui_lib
)

if(MSVC)
    target_compile_options(OptimizationDemo PRIVATE /W3)
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
    PROPERTY VS_STARTUP_PROJECT OptimizationDemo)