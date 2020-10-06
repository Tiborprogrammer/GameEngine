#include <iostream>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Application.h"

using namespace std;
/*
// glfwDestroyWindow(window);

void terminate_engine() {
    cout << "Terminating..." << endl;
    glfwTerminate();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    cout << key << endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina displays
    glViewport(0, 0, width, height);
}

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

int main2() {
    glfwSetErrorCallback(error_callback);

    cout << "Init..." << endl;

    if (!glfwInit()) {
        terminate_engine();
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", nullptr, nullptr);

    cout << "Creating window..." << endl;
    if (!window) {
        // Window or OpenGL context creation failed
        cout << "Creating window failed..." << endl;
        terminate_engine();
        return -1;
    }

    cout << "Creating window was successful..." << endl;

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize the OpenGL API with GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);

    glfwSwapInterval(1);

    glfwPollEvents();

    // Loop until the user closes the window
    while(!glfwWindowShouldClose(window)) {
        // Render here!
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    cout << "Finishing..." << endl;

    terminate_engine();
    return 0;
}
*/
int main() {
    Application application;

    application.run();

}