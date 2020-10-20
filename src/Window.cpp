//
// Created by Tibor Varga on 18/09/2020.
//

#include "Window.h"

// vertex = point/pos
const char* vertexShader = R"(
#version 400
layout (location=0) in vec3 vp;
void main() {
  gl_Position = vec4(vp, 1.0);
}
)";

const char* colorShader = R"(
#version 400
out vec4 frag_colour;
void main() {
  frag_colour = vec4(1.0, 0.2, 0.2, 1.0);
};
)";

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
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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


    glfwSetWindowUserPointer(this->glfwwindow, &eventProcessingFn);

    // Events
    glfwSetWindowCloseCallback(this->glfwwindow, windowCloseCallback);
    glfwSetWindowSizeCallback(this->glfwwindow, windowSizeCallback);
    glfwSetMouseButtonCallback(this->glfwwindow, mouseButtonCallback);
    glfwSetCursorPosCallback(this->glfwwindow, mouseMovedCallback);
    glfwSetKeyCallback(this->glfwwindow, keyCallback);
    initSuccessful = true;

    float vertexes[3][3] = {{-0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}, {0.5f,  0.5f, 0.0f}};

    unsigned int bufferId;
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*3, vertexes, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    // 0 = current buffer, number of dimensions in a vertex, type of positions, coordinate type(-1 to 1), size of each vertex(0 = auto)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


    unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexShader, nullptr);

    unsigned int colorShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(colorShaderId, 1, &colorShader, nullptr);

    glCompileShader(vertexShaderId);
    glCompileShader(colorShaderId);

    this->shaderProgramId = glCreateProgram();
    glAttachShader(this->shaderProgramId, vertexShaderId);
    glAttachShader(this->shaderProgramId, colorShaderId);

    glLinkProgram(this->shaderProgramId);



    /*unsigned int indicesArray[] = {0, 1, 2, 3};
    unsigned int indicesBufferId;
    glGenBuffers(1, &indicesBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*4, vertexes, GL_STATIC_DRAW);*/

    GLenum error_code = glGetError();
}

void Window::setEventProcessingFn(const EventProcessingFn &eventProcessingFn) {
    this->eventProcessingFn = eventProcessingFn;
}

void Window::update() {
    // Render here!
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)

    glBindVertexArray(vertexArrayId);
    glUseProgram(this->shaderProgramId);
    glDrawArrays(GL_TRIANGLES, 0, 3);


    GLenum error_code = glGetError();

    // Swap front and back buffers
    glfwSwapBuffers(this->glfwwindow);

    // Poll for and process events
    glfwPollEvents();
}
