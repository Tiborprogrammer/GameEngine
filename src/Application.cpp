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

        case EventType::MouseMoved : {
            float xPos = (float)event.eventData.mouseMovedData.x;
            float yPos = (float)window->getHeight() - ((float)event.eventData.mouseMovedData.y);
            mousePositionAbsolute = {xPos, yPos};
            std::cout << "Mouse moved: " << event.eventData.mouseMovedData.x << "  " << event.eventData.mouseMovedData.y << "\n";
            break;
        }

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


        window->startDraw();

        window->setColor({0.5, 0.5, 0}, 1);

        Vector2 mousePos = window->pixelToPercent((this->mousePositionAbsolute));
        float xMousePos = mousePos.x;
        float yMousePos = mousePos.y;

        Vector2 mouseControlledTriangle[3] = {{xMousePos, yMousePos}, {xMousePos + 0.5f, yMousePos}, {xMousePos, yMousePos - 0.5f}};

        window->drawTriangle(mouseControlledTriangle);

        window->setColor({0, 0.5, 0.5}, 1);

        Vector2 rect[2] = {{0, 0}, {0.9, 0.5}};
        window->drawRect(rect[0], rect[1]);

        window->endDraw();
    }
}

void Application::OnWindowClose(Event &e) {
    window->shutDown();
    running = false;
}
