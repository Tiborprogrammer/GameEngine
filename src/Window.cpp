//
// Created by Tibor Varga on 18/09/2020.
//

#include "Window.h"


/*
 * GL QUADS -> GL TRIANGLE
 * Swap Buffers (0)  ->   Swap Buffers (1)
 *
 *
 */



const char* vertex_shader_code = R"(
#version 400
layout (location=0) in vec3 vp;
void main() {
  gl_Position = vec4(vp, 1.0);
}
)";  // Shader responsible for transforming points (e.g. computing their final location)

const char* fragment_shader_code = R"(
#version 400
out vec4 frag_colour;
void main() {
  frag_colour = vec4(1.0, 0.2, 0.2, 1.0);
};
)";  // Shader responsible for producing the color of the point



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


    glfwSetWindowUserPointer(this->glfwwindow, &eventProcessingFn);

    // Events
    glfwSetWindowCloseCallback(this->glfwwindow, windowCloseCallback);
    glfwSetWindowSizeCallback(this->glfwwindow, windowSizeCallback);
    glfwSetMouseButtonCallback(this->glfwwindow, mouseButtonCallback);
    glfwSetCursorPosCallback(this->glfwwindow, mouseMovedCallback);
    glfwSetKeyCallback(this->glfwwindow, keyCallback);
    initSuccessful = true;

    float vertexes[3][3] = {
        {-0.5f, -0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f},
        {0.5f,  0.5f, 0.0f}
    };

    unsigned int bufferId;
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*3, vertexes, GL_STATIC_DRAW);


    // tell GL to only draw onto a pixel if the shape is closer to the viewer

    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof, nullptr);

//    unsigned int indicesArray[] = {0, 1, 2, 3};
//    unsigned int indicesBufferId;
//    glGenBuffers(1, &indicesBufferId);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferId);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*4, vertexes, GL_STATIC_DRAW);


    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);


    // Setup the vertex shader and the fragment shader
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &vertex_shader_code, nullptr);
    glCompileShader(vertex_shader_id);

    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragment_shader_code, nullptr);
    glCompileShader(fragment_shader_id);

    // "Now, these compiled shaders must be combined into a single, executable GPU shader programme.
    // We create an empty "program", attach the shaders, then link them together."
    this->shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fragment_shader_id);
    glAttachShader(shader_programme, vertex_shader_id);
    glLinkProgram(shader_programme);


}

void Window::setEventProcessingFn(const EventProcessingFn &eventProcessingFn) {
    this->eventProcessingFn = eventProcessingFn;
}

void Window::update() {

    // Render here!
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)


    // Use the shaders we created
    glUseProgram(shader_programme);
    glBindVertexArray(vertexArrayId);
    glDrawArrays(GL_TRIANGLES, 0, 3);


//    glColor3f(1.0f, 0.0f, 0.0f);
//    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, nullptr);


//    glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
//    glLoadIdentity(); //Reset the drawing perspective
//
//    // Draw a Red 1x1 Square centered at origin
//    glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
//    glColor3f(1.0f, 0.0f, 0.0f); // Red
//    glVertex2f(-0.5f, -0.5f);    // x, y
//    glVertex2f( 0.5f, -0.5f);
//    glVertex2f( 0.5f,  0.5f);
//    glVertex2f(-0.5f,  0.5f);
//    glEnd();
//
//    glFlush();  // Render now

    // Swap front and back buffers
    glfwSwapBuffers(this->glfwwindow);

    // Poll for and process events
    glfwPollEvents();
}
