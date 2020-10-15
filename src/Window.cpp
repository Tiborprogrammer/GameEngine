//
// Created by Tibor Varga on 18/09/2020.
//

#include "Window.h"

void windowCloseCallback(GLFWwindow* glfwWindow) {
    Event windowCloseEvent = Event();
    windowCloseEvent.eventType = EventType::WindowClose;
    windowCloseEvent.eventCategory = EventCategoryWindow;

    auto* eventProcessingFn = (Window::EventProcessingFn*)glfwGetWindowUserPointer(glfwWindow);
    (*eventProcessingFn)(windowCloseEvent);
}

void windowSizeCallback(GLFWwindow* glfwWindow, int width, int height) {
    Event windowResizeEvent = Event();
    windowResizeEvent.eventType = EventType::WindowResize;
    windowResizeEvent.eventCategory = EventCategoryWindow;
    windowResizeEvent.eventData.resizeData = {width, height};

    auto* eventProcessingFn = (Window::EventProcessingFn*)glfwGetWindowUserPointer(glfwWindow);
    (*eventProcessingFn)(windowResizeEvent);
}

void mouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods) {
    Event mouseButtonEvent = Event();
    mouseButtonEvent.eventType = (action == GLFW_PRESS ? EventType::MouseButtonPressed : EventType::MouseButtonReleased);
    mouseButtonEvent.eventCategory = EventCategoryMouseButton | EventCategoryMouse;
    mouseButtonEvent.eventData.button = button;

    auto* eventProcessingFn = (Window::EventProcessingFn*)glfwGetWindowUserPointer(glfwWindow);
    (*eventProcessingFn)(mouseButtonEvent);
}

void mouseMovedCallback(GLFWwindow* glfwWindow, double x, double y) {
    Event mouseMovedEvent = Event();
    mouseMovedEvent.eventType = EventType::MouseMoved;
    mouseMovedEvent.eventCategory = EventCategoryMouse;
    mouseMovedEvent.eventData.mouseMovedData = {x, y};


    auto* eventProcessingFn = (Window::EventProcessingFn*)glfwGetWindowUserPointer(glfwWindow);
    (*eventProcessingFn)(mouseMovedEvent);
}

void keyCallback(GLFWwindow* glfwWindow, int key, int scanCode, int action, int mods) {
    Event keyEvent = Event();
    keyEvent.eventCategory = EventCategoryMouse;
    if (action == GLFW_PRESS) {
        keyEvent.eventData.keyPressed = {key, true};
        keyEvent.eventType = EventType::KeyPressed;
    } else if (action == GLFW_REPEAT) {
        keyEvent.eventData.keyPressed = {key, false};
        keyEvent.eventType = EventType::KeyPressed;
    } else {
        keyEvent.eventData.keyReleased = key;
        keyEvent.eventType = EventType::KeyReleased;
    }


    auto* eventProcessingFn = (Window::EventProcessingFn*)glfwGetWindowUserPointer(glfwWindow);
    (*eventProcessingFn)(keyEvent);
}

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}
void Window::shutDown() {
    std::cout << "Terminating..." << std::endl;
    glfwTerminate();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina displays
    glViewport(0, 0, width, height);
}

Window::Window(const WindowProperties &windowProperties) : windowProperties(windowProperties) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        shutDown();
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    this->glfwwindow = glfwCreateWindow(windowProperties.width, windowProperties.height, windowProperties.title.c_str(), nullptr, nullptr);

    std::cout << "Creating window..." << std::endl;
    if (!glfwwindow) {
        // Window or OpenGL context creation failed
        std::cout << "Creating window failed..." << std::endl;
        shutDown();
        return;
    }

    std::cout << "Creating window was successful..." << std::endl;

    glfwMakeContextCurrent(glfwwindow);
    glfwSetFramebufferSizeCallback(glfwwindow, framebuffer_size_callback);

    // Initialize the OpenGL API with GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glfwSwapInterval(1);

    glfwPollEvents();


    glfwSetWindowUserPointer(this->glfwwindow, &eventProcessingFn);

    // Events
    glfwSetWindowCloseCallback(this->glfwwindow, windowCloseCallback);
    glfwSetWindowSizeCallback(this->glfwwindow, windowSizeCallback);
    glfwSetMouseButtonCallback(this->glfwwindow, mouseButtonCallback);
    glfwSetCursorPosCallback(this->glfwwindow, mouseMovedCallback);
    glfwSetKeyCallback(this->glfwwindow, keyCallback);
    initSuccessful = true;

}

void Window::setEventProcessingFn(const EventProcessingFn &eventProcessingFn) {
    this->eventProcessingFn = eventProcessingFn;
}

void Window::update() {
    // Render here!
    glClear(GL_COLOR_BUFFER_BIT);

    // Swap front and back buffers
    glfwSwapBuffers(this->glfwwindow);

    // Poll for and process events
    glfwPollEvents();
}
