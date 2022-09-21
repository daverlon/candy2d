cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(CXX_FLAGS "-Verbose -Wc++11-extensions -std=c++17")

set(EXECUTABLE_OUTPUT_PATH bin)

include_directories(
    #"/opt/homebrew/Cellar/sdl2/2.24.0/include"
    #"/opt/homebrew/Cellar/sdl2_ttf/2.20.1/include"
    "dependencies/include"
)

link_directories(
    #"/opt/homebrew/Cellar/sdl2/2.24.0/lib"
    #"/opt/homebrew/Cellar/sdl2_ttf/2.20.1/lib"
    "dependencies/lib"
)

project(sdl2_game)

file(GLOB SOURCES
    "src/main.cpp"
    "src/debugutils.cpp"
    "src/gamemath.cpp"

    "src/viewport.cpp"
    "src/sprite.cpp"
    "src/camera.cpp"
)

add_executable(sdl2_game ${SOURCES})

target_link_libraries(sdl2_game sdl2 sdl2_ttf sdl2_image)
