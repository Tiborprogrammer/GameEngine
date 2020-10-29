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

typedef float Vertex3[3];

struct Vector2 {
    float x;
    float y;
};

class Window {
    GLFWwindow* glfwwindow;
    GLuint vertexArrayId;
    GLuint shaderProgramId;
    GLuint triangleBufferId;

public:
    using EventProcessingFn = std::function<void(Event&)>;
    bool initSuccessful = false;
    WindowProperties windowProperties;
    EventProcessingFn eventProcessingFn;


    unsigned int getWidth() const { return windowProperties.width; };
    unsigned int getHeight() const { return windowProperties.height; };
    std::string getTitle() const { return windowProperties.title; };
    Window(const WindowProperties &windowProperties = WindowProperties());


    void drawTriangle(Vertex3 vertexes[3]);
    void drawRect(Vertex3 vertexes[2]);
    void startDraw();
    void endDraw();
    void setEventProcessingFn(const EventProcessingFn& eventProcessingFn);
    static void shutDown();

    Vector2 pixelToPercent(Vector2 position);
    float lerp(float percentage, float toMin, float toMax);
};


#endif //GAMEENGINE_WINDOW_H
