//
// Created by Tibor Varga on 03/09/2020.
//

#ifndef GAMEENGINE_WINDOW_H
#define GAMEENGINE_WINDOW_H

#include <string>
#include "events/Event.h"
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <utility>
#include <functional>

struct WindowProperties {
    WindowProperties(int width = 600, int height = 600, std::string title = "GameEngine") : width(width), height(height), title(std::move(title)) {}

    unsigned int width;
    unsigned  int height;
    std::string title;
};

class Window {
    GLFWwindow* glfwwindow;
    GLuint vertexArrayId;
    GLuint shaderProgramId;
public:
    using EventProcessingFn = std::function<void(Event&)>;
    bool initSuccessful = false;
    WindowProperties windowProperties;
    EventProcessingFn eventProcessingFn;


    unsigned int getWidth() const { return windowProperties.width; };
    unsigned int getHeight() const { return windowProperties.height; };
    std::string getTitle() const { return windowProperties.title; };
    Window(const WindowProperties &windowProperties = WindowProperties());


    void update();
    void setEventProcessingFn(const EventProcessingFn& eventProcessingFn);
    static void shutDown();
};


#endif //GAMEENGINE_WINDOW_H
