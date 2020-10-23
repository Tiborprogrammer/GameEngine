set(CMAKE_C_COMPILER "/Library/Developer/CommandLineTools/usr/bin/cc")
cmake_minimum_required(VERSION 3.17)
project(GameEngine)

set(CMAKE_CXX_STANDARD 20)
find_package(glfw3 3.3 REQUIRED)

add_executable(GameEngine src/main.cpp src/events/Event.h src/Window.cpp src/Window.h src/Application.cpp src/Application.h)

if (UNIX)
    set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl  ")
endif(UNIX)

add_custom_command(TARGET GameEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/shaders
        $<TARGET_FILE_DIR:GameEngine>/shaders)

# glad
set(GLAD_DIR "glad")
add_library("glad" "glad/src/glad.c")
target_include_directories("glad" PRIVATE "glad/include")
target_include_directories(GameEngine PRIVATE "glad/include")
target_link_libraries(GameEngine "glad" "glfw")