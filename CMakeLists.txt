cmake_minimum_required(VERSION 3.16)
project(SnakePro VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Snake.cpp
    src/Renderer.cpp
    src/InputHandler.cpp
    src/ScoreManager.cpp
)

add_executable(snake-pro ${SOURCES})

target_include_directories(snake-pro PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(snake-pro PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}
)

if(WIN32)
    target_link_libraries(snake-pro PRIVATE)
endif()
