//
// Created by Tibor Varga on 18/09/2020.
//

#include "Window.h"
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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

struct File {
    std::string file;
    bool failed;
};

File readFile(std::string name) {
    std::ifstream inFile(name);
    if (inFile.fail())
        return {"", true};

    std::stringstream buffer;
    buffer << inFile.rdbuf();

    return {buffer.str(), false};
}

GLuint buildShader(std::string name, GLenum shaderType) {
    File shaderSourceCode = readFile(name);
    if (shaderSourceCode.failed) {
        std::cout << "The file " << name << " was not found! \n";
        shaderSourceCode.file = "";
    }
    const char* arrayShaderSourceCode = shaderSourceCode.file.c_str();

    GLuint shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &arrayShaderSourceCode, nullptr);

    glCompileShader(shaderId);

    GLint compileStatus;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_TRUE)
        std::cout << name << " was successfully compiled \n";
    else {
        std::cout << name << " failed to compile! \n";
        char buffer[2000];
        glGetShaderInfoLog(shaderId, 2000, nullptr, buffer);
        std::cout << buffer << std::endl;
    }

    return shaderId;
}

Window::Window(const WindowProperties &windowProperties) : windowProperties(windowProperties) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        shutDown();
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
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

    glGenBuffers(1, &this->triangleBufferId);


    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, this->triangleBufferId);
    // 0 = current buffer, number of dimensions in a vertex, type of positions, coordinate type(-1 to 1), size of each vertex(0 = auto)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    GLuint fragmentShaderId = buildShader("shaders/fragmentShader.glsl", GL_FRAGMENT_SHADER);
    GLuint vertexShaderId = buildShader("shaders/vertexShader.glsl", GL_VERTEX_SHADER);

    this->shaderProgramId = glCreateProgram();
    glAttachShader(this->shaderProgramId, vertexShaderId);
    glAttachShader(this->shaderProgramId, fragmentShaderId);

    glLinkProgram(this->shaderProgramId);

    setCamera(Vector2(0, 1));
    /*unsigned int indicesArray[] = {0, 1, 2, 3};
    unsigned int indicesBufferId;
    glGenBuffers(1, &indicesBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*4, vertexes, GL_STATIC_DRAW);*/
}

void Window::setEventProcessingFn(const EventProcessingFn &eventProcessingFn) {
    this->eventProcessingFn = eventProcessingFn;
}

void Window::startDraw() {
    // Render here!
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)
}

void Window::endDraw() {
    // Swap front and back buffers
    glfwSwapBuffers(this->glfwwindow);

    // Poll for and process events
    glfwPollEvents();
}
void Window::drawTriangle(Vector2 vertexes[3]) {
    Vertex3 trianglePoints[3] = {{vertexes[0].x, vertexes[0].y, 0}, {vertexes[1].x, vertexes[1].y, 0}, {vertexes[2].x, vertexes[2].y, 0}};

    glUseProgram(this->shaderProgramId);

    glBindBuffer(GL_ARRAY_BUFFER, this->triangleBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*3, trianglePoints, GL_STATIC_DRAW);
    glBindVertexArray(vertexArrayId);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

Vector2 Window::pixelToPercent(Vector2 position) {
    float xPercentage = position.x / windowProperties.width;
    float yPercentage = position.y / windowProperties.height;
    return {lerp(xPercentage, -1, 1), lerp(yPercentage, -1, 1)};
}

float Window::lerp(float percentage, float min, float max) {
    return min + ((max - min) * percentage);
}

void Window::drawRect(Vector2 vertexes[2]) {
    Vector2 bottomLeft = vertexes[0];
    Vector2 topRight = vertexes[1];

    Vector2 triangle1Vertexes[3] = {{bottomLeft.x, topRight.y},
                                    {bottomLeft.x, bottomLeft.y},
                                    {topRight.x,   bottomLeft.y}};

    Vector2 triangle2Vertexes[3] = {{topRight.x,   bottomLeft.y},
                                    {topRight.x,   topRight.y},
                                    {bottomLeft.x, topRight.y}};

    drawTriangle(triangle1Vertexes);
    drawTriangle(triangle2Vertexes);
}

void Window::drawRect(Vector2 bottomLeft, Vector2 size) {
    Vector2 twoPositionsOfRect[2] = {bottomLeft, bottomLeft+size};

    drawRect(twoPositionsOfRect);
};

void Window::setColor(Vector3 color, float opacity) {
    GLint colorVarLocation = glGetUniformLocation(this->shaderProgramId, "color");
    glProgramUniform4f(this->shaderProgramId, colorVarLocation, color.x, color.y, color.z, opacity);
}

void Window::update() {
    this->startDraw();

    for (Layer* layer : this->layers) {
        layer->Update();
        layer->Render();
    }

    this->endDraw();
}

void Window::addLayer(Layer* layer) {
    this->layers.push_back(layer);
}

void Window::setCamera(Vector2 position) {
    // TODO:
    //  - keep track of camera matrix and make change to it over time
    //  - be able to reset the camera matrix
    //  - explain camera matrix logic
    //      - why is there an extra dimension
    //      - how does translation work
    //      - how does scaling work (scale 3 first coord or last one, but better to scale 3 first coordinates)
    //      - how does rotation work (restate equations and then re-express them as matrix multiplications)
    glm::mat4 projectionMatrix = glm::mat4(1.0);
    projectionMatrix = glm::translate(projectionMatrix, glm::vec3(-position.x, -position.y, 0));

    glUseProgram(this->shaderProgramId);
    GLint cameraMatrixLocation = glGetUniformLocation(this->shaderProgramId, "projectionMatrix");
    glUniformMatrix4fv(cameraMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void Window::rotateCamera(float angle) {
    glm::mat4 projectionMatrix = glm::mat4(1.0);
    projectionMatrix = glm::rotate(projectionMatrix, glm::radians(angle), glm::vec3(0, 0, 1));


    glUseProgram(this->shaderProgramId);
    GLint cameraMatrixLocation = glGetUniformLocation(this->shaderProgramId, "projectionMatrix");
    glUniformMatrix4fv(cameraMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}