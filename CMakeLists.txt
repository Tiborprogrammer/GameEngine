set(CMAKE_C_COMPILER "/Library/Developer/CommandLineTools/usr/bin/cc")
cmake_minimum_required(VERSION 3.17)
project(GameEngine)

set(CMAKE_CXX_STANDARD 20)
find_package(glfw3 3.3.2 REQUIRED)

add_executable(GameEngine src/main.cpp src/events/Event.h src/Window.cpp src/Window.h src/Application.cpp src/Application.h)

# glad
set(GLAD_DIR "glad")
add_library("glad" "glad/src/glad.c")
target_include_directories("glad" PRIVATE "glad/include")
target_include_directories(GameEngine PRIVATE "glad/include")
target_link_libraries(GameEngine "glad" "glfw")