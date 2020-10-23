//
// Created by Tibor Varga on 03/09/2020.
//

#include "Application.h"

#include <memory>

void Application::receiveEvent(Event& event) {
    switch (event.eventType) {
        case EventType::WindowClose :
            OnWindowClose(event);
            break;

        case EventType::WindowResize :
            std::cout << "Resize: " << event.eventData.resizeData.width << "  " << event.eventData.resizeData.height << "\n";
            break;

        case EventType::MouseMoved :
            std::cout << "Mouse moved: " << event.eventData.mouseMovedData.x << "  " << event.eventData.mouseMovedData.y << "\n";
            break;

        case EventType::MouseButtonReleased:
            std::cout << "Mouse button released" << "\n";
            break;

        case EventType::MouseButtonPressed:
            std::cout << "Mouse button pressed" << "\n";
            break;

        case EventType::KeyPressed:
            if (event.eventData.keyPressed.key == GLFW_KEY_LEFT)
                left = true;
            if (event.eventData.keyPressed.key == GLFW_KEY_RIGHT)
                right = true;
            if (event.eventData.keyPressed.key == GLFW_KEY_UP)
                up = true;
            if (event.eventData.keyPressed.key == GLFW_KEY_DOWN)
                down = true;
            if (event.eventData.keyPressed.key == GLFW_KEY_SPACE)
                movementSpeed += 0.5;

            std::cout << "Key pressed: " << event.eventData.keyPressed.key << "  " << event.eventData.keyPressed.isFirstTime << "\n";
            break;

        case EventType::KeyReleased:
            if (event.eventData.keyPressed.key == GLFW_KEY_LEFT)
                left = false;
            if (event.eventData.keyPressed.key == GLFW_KEY_RIGHT)
                right = false;
            if (event.eventData.keyPressed.key == GLFW_KEY_UP)
                up = false;
            if (event.eventData.keyPressed.key == GLFW_KEY_DOWN)
                down = false;

            break;

        default:
            break;


    }
}

Application::Application() {
    window = std::make_unique<Window>();
    window->setEventProcessingFn([this](Event& event) {
        this->receiveEvent(event);
    });
}

void Application::run() {
    while (running) {
        if (left)
            xOffset -= 0.001 * movementSpeed;
        if (right)
            xOffset += 0.001 * movementSpeed;

        if (up)
            yOffset += 0.001 * movementSpeed;
        if (down)
            yOffset -= 0.001 * movementSpeed;

        Vertex3 vertexes[3] = {{-0.5f + xOffset, -0.5f + yOffset, 0.0f}, {0.5f + xOffset, -0.5f + yOffset, 0.0f}, {0.5f + xOffset, 0.5f + yOffset, 0.0f}};
        window->startDraw();
        window->drawTriangle(vertexes);
        window->endDraw();
    }
}

void Application::OnWindowClose(Event &e) {
    window->shutDown();
    running = false;
}
