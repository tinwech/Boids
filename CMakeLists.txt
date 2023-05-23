cmake_minimum_required(VERSION 3.0.0)
project(boids VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

include_directories(include)

add_executable(boids
    src/main.cpp
    src/boid.cpp
    src/flock.cpp
    src/glad.c
)

target_link_libraries(boids dl
    glfw
)