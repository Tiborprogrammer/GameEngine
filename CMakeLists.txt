cmake_minimum_required(VERSION 3.17)
project(GameEngine)

set(CMAKE_CXX_STANDARD 14)
find_package(glfw3 3.3.2 REQUIRED)

add_executable(GameEngine main.cpp linmath.h)

# glad
set(GLAD_DIR "glad")
add_library("glad" "glad/src/glad.c")
target_include_directories("glad" PRIVATE "glad/include")
target_include_directories(GameEngine PRIVATE "glad/include")
target_link_libraries(GameEngine "glad" "glfw")