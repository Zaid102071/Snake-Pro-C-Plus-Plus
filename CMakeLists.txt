cmake_minimum_required(VERSION 3.20)
project(Snake-Pro VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Snake.cpp
    src/Food.cpp
    src/ScoreManager.cpp
    src/Renderer.cpp
)

set(HEADERS
    include/Constants.hpp
    include/Game.hpp
    include/Snake.hpp
    include/Food.hpp
    include/ScoreManager.hpp
    include/Renderer.hpp
)

add_executable(snake-pro ${SOURCES})

target_include_directories(snake-pro PRIVATE include)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

target_link_libraries(snake-pro PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

if(MSVC)
    target_compile_options(snake-pro PRIVATE /W4 /WX)
else()
    target_compile_options(snake-pro PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_compile_features(snake-pro PRIVATE cxx_std_17)

install(TARGETS snake-pro RUNTIME DESTINATION bin)
install(DIRECTORY assets/ DESTINATION assets)
