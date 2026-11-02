cmake_minimum_required(VERSION 3.16)
project(shooter LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(shooter src/main.cpp src/camera.cpp src/glad.c src/Shader.cpp src/mesh.cpp src/texture.cpp src/player.cpp src/Game.cpp src/bullet.cpp)
target_include_directories(shooter PRIVATE src include)
target_link_libraries(shooter PRIVATE SDL2::SDL2 glm::glm)
