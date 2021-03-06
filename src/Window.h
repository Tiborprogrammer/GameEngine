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
#include "Layer.h"
#include <vector>
#include <unordered_map>
#include <glm/detail/type_mat4x4.hpp>

struct WindowProperties {
    WindowProperties(int width = 600, int height = 600, std::string title = "GameEngine") : width(width), height(height), title(std::move(title)) {}

    unsigned int width;
    unsigned  int height;
    std::string title;
};

typedef float Vertex3[3];
typedef float Vertex5[5];

struct Vector2 {
    float x;
    float y;

    Vector2(Vertex3 vertex3) {
        x = vertex3[0];
        y = vertex3[1];
    }

    Vector2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    Vector2 operator+(const Vector2 &other) {
        return {x+other.x, y+other.y};
    }
};

struct Vector3 {
    float x;
    float y;
    float z;

    Vector3(Vertex3 vertex3) {
        x = vertex3[0];
        y = vertex3[1];
        z = vertex3[2];
    }

    Vector3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector2 toVector2() {
        return Vector2(x, y);
    }

    Vector3 operator+(const Vector3 &other) {
        return {x+other.x, y+other.y, z+other.z};
    }
};


struct Vector5 {
    float x;
    float y;
    float z;
    float u;
    float v;

    Vector5(Vertex5 vertex5) {
        x = vertex5[0];
        y = vertex5[1];
        z = vertex5[2];
        u = vertex5[3];
        v = vertex5[4];
    }

    Vector5(float x, float y, float z, float u, float v) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->u = u;
        this->v = v;
    }

    Vector2 toVector2() {
        return Vector2(x, y);
    }

    Vector5 operator+(const Vector5 &other) {
        return {x+other.x, y+other.y, z+other.z, u+other.u, v+other.v};
    }
};


class Window {
    GLFWwindow* glfwwindow;
    GLuint vertexArrayId;
    GLuint mainShaderProgramId;
    GLuint textureShaderProgramId;
    GLuint triangleTextureBufferId;
    GLuint textureVertexArrayId;
    GLuint triangleBufferId;
    std::vector<Layer*> layers;
    glm::mat4 projectionMatrix = glm::mat4(1.0);
    std::unordered_map<std::string, GLuint> texturesBufferId;


public:
    using EventProcessingFn = std::function<void(Event&)>;
    bool initSuccessful = false;
    WindowProperties windowProperties;
    EventProcessingFn eventProcessingFn;


    unsigned int getWidth() const { return windowProperties.width; };
    unsigned int getHeight() const { return windowProperties.height; };
    std::string getTitle() const { return windowProperties.title; };
    explicit Window(const WindowProperties &windowProperties = WindowProperties());


    void drawTriangle(Vector2 vertexes[3]) const;
    void drawRect(Vector2 vertexes[2]);
    void drawRect(Vector2 bottomLeft, Vector2 size);
    void startDraw();
    void endDraw();
    void setEventProcessingFn(const EventProcessingFn& eventProcessingFn);
    static void shutDown();
    void setColor(Vector3 color, float opacity);
    void update();
    void addLayer(Layer* layer);
    void translateCamera(Vector2 position);
    void rotateCamera(float angle);
    void resetCamera();
    void drawTextureTriangle(Vector5 triangleVertexes[3], const std::string& textureFile);

    Vector2 pixelToPercent(Vector2 position);
    float lerp(float percentage, float toMin, float toMax);

private:
    void setCamMatrixInShader();
    static GLuint createShaderProgram(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName);
    GLuint loadTexture(const std::string& textureFile);
    void setupVertexArrayAndBufferForTextures();
};


#endif //GAMEENGINE_WINDOW_H
