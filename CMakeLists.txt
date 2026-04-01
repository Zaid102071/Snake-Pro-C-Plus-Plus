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

if(WIN32)
    target_link_libraries(snake-pro PRIVATE)
endif()

install(TARGETS snake-pro DESTINATION bin)
