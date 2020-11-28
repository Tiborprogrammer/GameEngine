//
// Created by Tibor Varga on 03/09/2020.
//

#include "Application.h"
#include "BackgroundLayer.h"
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
    window = new Window();
    window->setEventProcessingFn([this](Event& event) {
        this->receiveEvent(event);
    });


    BackgroundLayer* backgroundLayer = new BackgroundLayer(*window);

    window->addLayer(backgroundLayer);

}


void Application::run() {
    float angle = 0;
    while (running) {
        angle += 1;
        window->rotateCamera(angle);
        window->update();
    }
}

void Application::OnWindowClose(Event &e) {
    window->shutDown();
    running = false;
}
