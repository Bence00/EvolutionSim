cmake_minimum_required(VERSION 3.16)
project(MyApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

set(IMGUI_DIR       "${CMAKE_SOURCE_DIR}/external/imgui-1.91.9")
set(IMGUI_SFML_DIR  "${CMAKE_SOURCE_DIR}/external/imgui-sfml-3.0")

set(IMGUI_INCLUDE_IMPL_EXAMPLES OFF)
add_subdirectory(${IMGUI_SFML_DIR})

file(GLOB_RECURSE SRC_FILES external/imgui-1.91.9/*.cpp ...)
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        ImGui-SFML::ImGui-SFML
)
