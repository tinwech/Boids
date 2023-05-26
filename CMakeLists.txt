cmake_minimum_required(VERSION 3.0.0)
project(boids VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

include_directories("include")

file(GLOB SRC
    "src/*.cpp"
    "src/*.c"
    "include/imgui/*.cpp"
)

message(SRC="${SRC}")

add_executable(boids ${SRC})

target_link_libraries(boids dl
    glfw
)