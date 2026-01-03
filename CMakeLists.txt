cmake_minimum_required(VERSION 3.20)
project(OpenWorldOptimization)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenGL 찾기 (시스템에 기본 설치됨)
find_package(OpenGL REQUIRED)

# GLFW 자동 다운로드
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

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

# Assimp
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

# ImGui
# FetchContent_Declare(
#     imgui
#     GIT_REPOSITORY https://github.com/ocornut/imgui.git
#     GIT_TAG v1.89.9
# )
# FetchContent_MakeAvailable(imgui)

# 소스 파일 수집
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/**/*.cpp
)

# 실행 파일
add_executable(OptimizationDemo
    ${SOURCES}
    external/glad/src/glad.c
)

# Include 경로
target_include_directories(OptimizationDemo PRIVATE
    src
    external/glad/include
)

# 라이브러리 링크
target_link_libraries(OptimizationDemo PRIVATE
    OpenGL::GL
    glfw
    glm::glm
    assimp::assimp
)

# Windows에서 콘솔 숨기기 (Release 빌드만)
if(MSVC)
    target_compile_options(OptimizationDemo PRIVATE /W3)
endif()

# Visual Studio 시작 프로젝트 설정
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
    PROPERTY VS_STARTUP_PROJECT OptimizationDemo)