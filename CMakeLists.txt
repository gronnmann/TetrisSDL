cmake_minimum_required(VERSION 3.24)
project(TetrisSDL)

set(CMAKE_CXX_STANDARD 17)


find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(TetrisSDL main.cpp TetrisWindow.cpp Tetromino.cpp ResourceManager.cpp)
target_include_directories(TetrisSDL PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})
target_link_libraries(TetrisSDL ${SDL2_LIBRARIES} ${SDL2_MIXER_LIBRARIES} /usr/local/lib/libSDL2_mixer.dylib
        /usr/local/lib/libSDL2_ttf.dylib /usr/local/lib/libSDL2_image.dylib)